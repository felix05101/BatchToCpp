#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

std::wstring wrapWithSystem(const std::wstring& line) {
    return L"system(\"" + line + L"\");";
}

int main() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Batch Files (*.bat)\0*.bat\0Newcummer\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        std::wstring inputFilePath(fileName);
        std::wifstream inputFile(inputFilePath);
        if (inputFile) {
            std::wstring outputFilePath = inputFilePath + L"_output.txt";
            std::wofstream outputFile(outputFilePath);
            if (outputFile) {
                std::wstring line;
                while (std::getline(inputFile, line)) {
                    std::wstring wrappedLine = wrapWithSystem(line);
                    outputFile << wrappedLine << std::endl;
                }
                outputFile.close();

            }

        }

    }


    return 0;
}
