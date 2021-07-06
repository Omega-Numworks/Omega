# themes_manager.py
# Authors:
#   - Maxime "M4x1m3" FRIESS
#   - Quentin Guidee

import argparse
import json
import os
import shutil
import subprocess
import sys
import textwrap


def get_env():
    return dict(os.environ, GIT_ASKPASS="/bin/true")


def check_for_git():
    try:
        output = subprocess.check_output(["git", "--version"])
        return True
    except FileNotFoundError:
        print("ERROR: Git not found! Can't download theme from the internet.",
              file=sys.stderr)
        return False


def check_git_remote(remote_url):
    try:
        output = subprocess.check_output(["git", "ls-remote", remote_url],
                                         env=get_env(),
                                         stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        print("ERROR: Invalid remote {}!".format(remote_url),
              file=os.stderr)
        return False


def git_pull(folder):
    try:
        print("Updating {}...".format(folder),
              file=sys.stderr)
        output = subprocess.check_output(["git", "-C", folder, "pull"],
                                         env=get_env(),
                                         stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        return False


def git_clone(url, folder):
    try:
        print("Cloning {} into {}...".format(url, folder),
              file=sys.stderr)
        output = subprocess.check_output(["git", "clone", url, folder, "--recurse-submodules"],
                                         env=get_env(),
                                         stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        return False


def get_icons_list():
    """
    Load icon list from file.
    """
    icon_list_path = (os.path.dirname(os.path.realpath(__file__))
                      + os.path.sep
                      + "icons.json")

    with open(icon_list_path, "r") as json_file:
        return json.load(json_file)


def get_data(theme, path):
    """
    Load theme from file.
    """
    theme_file_path = path + os.path.sep + theme + ".json"

    try:
        with open(theme_file_path, "r") as json_file:
            data = json.load(json_file)
    except FileNotFoundError:
        print("ERROR: Theme {} doesn't exist!".format(theme),
              file=sys.stderr)
        sys.exit(3)

    return data


def write_palette_h(data, file_p):
    """
    Write the header to file_p.
    """
    header = """#ifndef ESCHER_PALETTE_H
    #define ESCHER_PALETTE_H

    #include <kandinsky/color.h>
    #include <stddef.h>

    class Palette {
    public:
    """
    file_p.write(textwrap.dedent(header))

    line_template = "  constexpr static KDColor {name} = KDColor::RGB24(0x{value});"

    try:
        if data["version"] == 2:
            for key, value in data["colors"].items():
                for sub_key, sub_value in value.items():
                    if key == "Main":
                        name = sub_key
                    elif sub_key == "Main":
                        name = key
                    else:
                        name = key + sub_key

                    formatted_line = line_template.format(name=name,
                                                          value=sub_value)
                    file_p.write(formatted_line + "\n")
    except KeyError:
        print("THEME   If you are this theme creator, "
              "please consider updating the JSON theme file")

        for key, value in data["colors"].items():
            if isinstance(key, str):
                formatted_line = line_template.format(name=key,
                                                      value=value)
                file_p.write(formatted_line + "\n")
            else:
                for sub_key, sub_value in data["colors"][key].items():
                    formatted_line = line_template.format(name=key + sub_key,
                                                          value=sub_value)
                    file_p.write(formatted_line + "\n")

    # Default values - Sometimes never used.
    default_values = """      constexpr static KDColor YellowDark = KDColor::RGB24(0xffb734);
      constexpr static KDColor YellowLight = KDColor::RGB24(0xffcc7b);
      constexpr static KDColor PurpleBright = KDColor::RGB24(0x656975);
      constexpr static KDColor PurpleDark = KDColor::RGB24(0x414147);
      constexpr static KDColor GrayWhite = KDColor::RGB24(0xf5f5f5);
      constexpr static KDColor GrayBright = KDColor::RGB24(0xececec);
      constexpr static KDColor GrayMiddle = KDColor::RGB24(0xd9d9d9);
      constexpr static KDColor GrayDark = KDColor::RGB24(0xa7a7a7);
      constexpr static KDColor GrayVeryDark = KDColor::RGB24(0x8c8c8c);
      constexpr static KDColor Select = KDColor::RGB24(0xd4d7e0);
      constexpr static KDColor SelectDark = KDColor::RGB24(0xb0b8d8);
      constexpr static KDColor WallScreen = KDColor::RGB24(0xf7f9fa);
      constexpr static KDColor WallScreenDark = KDColor::RGB24(0xe0e6ed);
      constexpr static KDColor SubTab = KDColor::RGB24(0xb8bbc5);
      constexpr static KDColor LowBattery = KDColor::RGB24(0xf30211);
      constexpr static KDColor Red = KDColor::RGB24(0xff000c);
      constexpr static KDColor RedLight = KDColor::RGB24(0xfe6363);
      constexpr static KDColor Magenta = KDColor::RGB24(0xff0588);
      constexpr static KDColor Turquoise = KDColor::RGB24(0x60c1ec);
      constexpr static KDColor Pink = KDColor::RGB24(0xffabb6);
      constexpr static KDColor Blue = KDColor::RGB24(0x5075f2);
      constexpr static KDColor BlueLight = KDColor::RGB24(0x718fee);
      constexpr static KDColor Orange = KDColor::RGB24(0xfe871f);
      constexpr static KDColor Green = KDColor::RGB24(0x50c102);
      constexpr static KDColor GreenLight = KDColor::RGB24(0x52db8f);
      constexpr static KDColor Brown = KDColor::RGB24(0x8d7350);
      constexpr static KDColor Purple = KDColor::RGB24(0x6e2d79);
      constexpr static KDColor BlueishGrey = KDColor::RGB24(0x919ea4);
      constexpr static KDColor Cyan = KDColorBlue;
    """
    file_p.write(textwrap.dedent(default_values))

    file_p.write("  constexpr static KDColor DataColor[] = {Red, Blue, Green, YellowDark, Magenta, Turquoise, Pink, Orange};\n")
    file_p.write("  constexpr static KDColor DataColorLight[] = {RedLight, BlueLight, GreenLight, YellowLight};\n")

    file_p.write("  constexpr static KDColor AtomColor[] = {\n")
    file_p.write("    AtomUnknown, AtomAlkaliMetal, AtomAlkaliEarthMetal, AtomLanthanide, AtomActinide, AtomTransitionMetal,\n")
    file_p.write("    AtomPostTransitionMetal, AtomMetalloid, AtomHalogen, AtomReactiveNonmetal, AtomNobleGas\n")
    file_p.write("  };\n\n")

    file_p.write("  constexpr static size_t numberOfDataColors() { return sizeof(DataColor)/sizeof(KDColor); }\n")
    file_p.write("  constexpr static size_t numberOfLightDataColors() { return sizeof(DataColorLight)/sizeof(KDColor); }\n")
    file_p.write("  static KDColor nextDataColor(int * colorIndex);\n")
    file_p.write("};\n\n")

    file_p.write("#endif\n")


def handle_git(args):
    output_folder = os.path.basename(args.repo)
    output_folder = (output_folder[:-4]  # Remove .git extension if present.
                     if output_folder.endswith(".git")
                     else output_folder)

    output_path = "{dir}{sep}themes{sep}git{sep}{folder}".format(dir=os.path.dirname(os.path.realpath(__file__)),
                                                                 folder=output_folder,
                                                                 sep=os.path.sep)

    if not args.icon:  # Assume .h is called before icons, avoiding lot of pull for nothing.
        if os.path.exists(output_path):  # If directory exists, try to pull.
            if not git_pull(output_path):  # If can't pull, delete and re-clone.
                shutil.rmtree(output_path)
                git_clone(args.repo, output_path)
        else:  # If directory doesn't exist, clone.
            git_clone(args.repo, output_path)

    handle_theme(args, output_path)


def handle_theme(args, path):
    data = get_data(args.theme, path)

    if args.icon:
        # Get the icon in the icon theme folder.
        icons = get_icons_list()

        icon_path = "{path}{sep}{icons}{sep}{}".format(icons[args.output.replace(args.build_dir, "")],
                                                       icons=data["icons"],
                                                       path=path,
                                                       sep=os.path.sep)

        # Check if the file exists.
        if os.path.isfile(icon_path):
            # If yes, copy from theme.
            shutil.copyfile(icon_path, args.output)
        else:  # If no, copy from src.
            print(" (!!)   Icon {} not found in icon theme {icons}. "
                  "Using default!").format(icons[args.output.replace(args.build_dir, "")],
                                           icons=data["icons"])
            shutil.copyfile(args.output.replace(args.build_dir, ""),
                            args.output)
    else:
        if args.stdout:
            write_palette_h(data, sys.stdout)
        else:
            with open(args.output, "w") as palette_file:
                write_palette_h(data, palette_file)


def main(args):
    if args.list:
        print(" ==== Local themes ====")

        path = "{dir}{sep}themes{sep}local".format(dir=os.path.dirname(os.path.realpath(__file__)),
                                                   sep=os.path.sep)

        for file_info in os.listdir(path):
            if file_info.endswith(".json"):
                filename = os.path.splitext(file_info)[0]
                print(filename)

        sys.exit(0)
    else:
        if args.theme is None or args.repo is None:
            print("Please specify repo and theme or use --list!",
                  file=sys.stderr)
            sys.exit(2)

    if args.repo == "local":
        path = "{dir}{sep}themes{sep}local".format(dir=os.path.dirname(os.path.realpath(__file__)),
                                                   sep=os.path.sep)

        handle_theme(args, path)
    else:
        if check_for_git():
            if check_git_remote(args.repo):
                handle_git(args)
            else:
                sys.exit(5)
        else:
            sys.exit(6)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process the themes.")
    parser.add_argument("repo", nargs="?", help='git remote from wtich to get the themes from. Set to "local" for included themes')
    parser.add_argument("theme", nargs="?", help="the name of the theme")
    parser.add_argument("output", nargs="?", help="path to the output header file")
    parser.add_argument("build_dir", nargs="?", help="path to the output folder")
    parser.add_argument("-l", "--list", help="list locals themes", action="store_true")
    parser.add_argument("-i", "--icon", help="outputs an icon instead of a header", action="store_true")
    parser.add_argument("--stdout", help="print palette.h to stdout", action="store_true")

    args = parser.parse_args()
    main(args)
