#!/bin/env python3

# themes_manager.py
# Authors:
#   - Maxime "M4x1m3" FRIESS
#   - Quentin Guidee

import sys
import argparse
import os
import json
import shutil
import subprocess

def openrel(path, mode):
    return open(os.path.join(os.path.dirname(__file__), path), mode)

def check_for_git():
    try:
        output = subprocess.check_output(["git", "--version"])
        return True
    except FileNotFoundError:
        print("ERROR: Git not found! Can't download theme from the internet.", file=sys.stderr)
        return False


def check_git_remote(remote_url):
    try:
        output = subprocess.check_output(["git", "ls-remote", remote_url], env=dict(os.environ, GIT_ASKPASS="/bin/true"), stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        print("ERROR: Invalid remote " + remote_url + "!", file=os.stderr)
        return False


def git_pull(folder):
    try:
        print("Updating " + folder + "...", file=sys.stderr)
        output = subprocess.check_output(["git", "-C", folder, "pull"], env=dict(os.environ, GIT_ASKPASS="/bin/true"), stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        return False


def git_clone(url, folder):
    try:
        print("Cloning " + url + " into " + folder + "...", file=sys.stderr)
        output = subprocess.check_output(["git", "clone", url, folder, "--recurse-submodules"], env=dict(os.environ, GIT_ASKPASS="/bin/true"), stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        return False


def get_icons_list():
    """
    Load icon list from file
    """
    with open(os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "icons.json", "r") as json_file:
        data = json.load(json_file)

    return data


def get_data(theme, path):
    """
    Load theme from file
    """
    try:
        with open(path + os.path.sep + theme + ".json", "r") as json_file:
            data = json.load(json_file)
    except FileNotFoundError:
        print("ERROR: Theme " + theme + " doesn't exist!", file=sys.stderr)
        sys.exit(3)

    return data

def write_palette_cpp(data, file_p):
    with openrel("template/palette.cpp.start", "r") as f:
        file_p.write(f.read())

    for key in data["colors"].keys():
        if type(data["colors"][key]) is str:
            file_p.write("PALETTE_CONST KDColor Palette::" + key + " = KDColor::RGB24(0x" + data["colors"][key] + ");\n")
        else:
            for sub_key in data["colors"][key].keys():
                file_p.write("PALETTE_CONST KDColor Palette::" + key + sub_key + " = KDColor::RGB24(0x" + data["colors"][key][sub_key] + ");\n")
    
    with openrel("template/palette.cpp.end", "r") as f:
        file_p.write(f.read())

def write_palette_h(data, file_p):
    with openrel("template/palette.h.start", "r") as f:
        file_p.write(f.read())

    for key in data["colors"].keys():
        if type(data["colors"][key]) is str:
            file_p.write("  PALETTE_CONST static KDColor " + key + ";\n")
        else:
            for sub_key in data["colors"][key].keys():
                file_p.write("  PALETTE_CONST static KDColor " + key + sub_key + ";\n")
    
    with openrel("template/palette.h.end", "r") as f:
        file_p.write(f.read())

def handle_git(args):
    output_folder = os.path.basename(args.repo)
    output_folder = output_folder[:-4] if output_folder.endswith(".git") else output_folder  # Remove .git extension if present.

    output_path = os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "themes" + os.path.sep + "git" + os.path.sep + output_folder

    if (not args.icon):  # Assume .h is called before icons, avoiding lot of pull for nothing.
        if (os.path.exists(output_path)):  # If directory exists, try to pull
            if (not git_pull(output_path)):  # If can't pull, delete and re-clone.
                shutil.rmtree(output_path)
                git_clone(args.repo, output_path)
        else:  # If directory doesn't exist, clone.
            git_clone(args.repo, output_path)

    handle_theme(args, output_path)


def handle_theme(args, path):
    data = get_data(args.theme, path)

    if (args.icon):
        # Get the icon in the icon theme folder
        icons = get_icons_list()

        icon_path = path + os.path.sep + data["icons"] + os.path.sep + icons[args.output.replace(args.build_dir, "")]

        # Check if the file exists
        if os.path.isfile(icon_path):
            # If yes, copy from theme
            shutil.copyfile(icon_path, args.output)
        else:
            # If no, copy from src
            print(" (!!)   Icon " + icons[args.output.replace(args.build_dir, "")] + " not found in icon theme " + data["icons"] + ". Using default!")
            shutil.copyfile(args.output.replace(args.build_dir, ""), args.output)
    else:
        if (args.stdout):
            if (args.cpp):
                write_palette_cpp(data, sys.stdout)
            else:
                write_palette_h(data, sys.stdout)
        else:
            with open(args.output, "w") as palette_file:
                if (args.cpp):
                    write_palette_cpp(data, palette_file)
                else:
                    write_palette_h(data, palette_file)


def main(args):
    if (args.list):
        print(" ==== Local themes ====")
        for file_info in os.listdir(os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "themes" + os.path.sep + "local"):
            if (file_info.endswith(".json")):
                filename = os.path.splitext(file_info)[0]
                print(filename)
        sys.exit(0)
    else:
        if (args.theme == None or args.repo == None):
            print("Please specify repo and theme or use --list!", file=sys.stderr)
            sys.exit(2)

    if args.repo == "local":
        handle_theme(args, os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "themes" + os.path.sep + "local")
    else:
        if check_for_git():
            if (check_git_remote(args.repo)):
                handle_git(args)
            else:
                sys.exit(5)
        else:
            sys.exit(6)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process the themes.")
    parser.add_argument("repo", nargs="?", help="git remote from wtich to get the themes from. Set to \"local\" for included themes")
    parser.add_argument("theme", nargs="?", help="the name of the theme")
    parser.add_argument("output", nargs="?", help="path to the output header file")
    parser.add_argument("build_dir", nargs="?", help="path to the output folder")
    parser.add_argument("-l", "--list", help="list locals themes", action="store_true")
    parser.add_argument("-i", "--icon", help="outputs an icon instead of a header", action="store_true")
    parser.add_argument("-c", "--cpp", help="generate cpp instead of header", action="store_true")
    parser.add_argument("--stdout", help="print palette.h to stdout", action="store_true")

    args = parser.parse_args()
    main(args)
