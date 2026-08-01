
namespace Ion {
namespace Device {
namespace USB {

const char* stringDescriptor(int slot) {
	if (slot == 1) {
		return "@Flash/0x90000000/08*004Kg,01*032Kg,63*064Kg";
	} else if (slot == 2) {
		return "@Flash/0x90400000/64*064Kg";
	} else if (slot == 3) {
		return "@Flash/0x90000000/08*004Kg,01*032Kg,63*064Kg,64*064Kg";
	} else {
		return "";
	}
}

}
}
}
