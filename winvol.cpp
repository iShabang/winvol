#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ole32.lib")

void logHRESULT(HRESULT result) {
    std::cout << "HRESULT: ";
    switch (result) {
        case E_POINTER:
            std::cout << "E_POINTER: Device is NULL";
            break;
        case E_INVALIDARG:
            std::cout << "E_INVALIDARG: Invalid dataFlow or role argument";
            break;
        case E_NOTFOUND:
            std::cout << "E_NOTFOUND: No device available";
            break;
        case E_OUTOFMEMORY:
            std::cout << "E_OUTOFMEMORY: Out of memory";
            break;
        case REGDB_E_CLASSNOTREG:
            std::cout << "REGDB_E_CLASSNOTREG";
            break;
        case CLASS_E_NOAGGREGATION:
            std::cout << "CLASS_E_NOAGGREGATION";
            break;
        case E_NOINTERFACE:
            std::cout << "E_NOINTERFACE";
            break;
        default:
            std::cout << "Unknown result: " << result;
    }
    std::cout << "\n";
}

void printUsage() {
    std::cout << "WINVOL\n";
    std::cout << "Sets the master volume of the default system audio device";
    std::cout << "Arguments: scalar value between 0 and 1\n";
    std::cout << "Set the volume to 20%\n";
    std::cout << "winvol.exe 0.2\n";
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printUsage();
        return 0;
    }
    float arg_volume = 0;
    try {
        std::string vol_string = argv[1];
        arg_volume = std::stof(vol_string);
    } catch(std::invalid_argument) {
        std::cout << "Invalid floating point argument" << std::endl;
        printUsage();
        return -1;
    } catch(std::out_of_range) {
        std::cout << "Float out of range" << std::endl;
        printUsage();
        return -1;
    }

    if (arg_volume < 0 || arg_volume > 1.0) {
        std::cout << "Value out of range. Expected value between 0 and 1.\n";
        return -1;
    }

    // Initialize the COM library. Required to get access to COM interfaces.
    HRESULT hr = CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        std::cout << "Failed to init COM. Error Code: " << hr << std::endl;
        return -1;
    }

    // Grab a reference to the device enumerator. This will allow us to list all the devices or
    // get the default one.
    IMMDeviceEnumerator *enumerator = NULL;
    HRESULT result = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL,
        CLSCTX_ALL, __uuidof(enumerator),
        reinterpret_cast<void**>(&enumerator));
    if (FAILED(result)) {
        std::cout << "Failed to get enumerator\n";
        logHRESULT(result);
        return -1;
    }


    // Finaly get a reference to the audio device. In this case, we want the default system device.
    IMMDevice *device = NULL;
    result = enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    if (FAILED(result)) {
        std::cout << "Failed to get device\n";
        logHRESULT(result);
        return -1;
    }

    // Grab a reference to the volume control interface
    IAudioEndpointVolume *volume = NULL;
    result = device->Activate(__uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,NULL,
        reinterpret_cast<void**>(&volume));
    if (FAILED(result)) {
        std::cout << "Failed to get volume control interface";
        logHRESULT(result);
        return -1;
    }

    // Set the master volume level
    result = volume->SetMasterVolumeLevelScalar(arg_volume, NULL);    
    if (FAILED(result)) {
        std::cout << "Failed to set master volume";
        logHRESULT(result);
        return -1;
    }

    std::cout << "Master volume set to " << arg_volume << std::endl;

    CoUninitialize();
    return 0;
}