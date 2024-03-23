#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

std::wstring escapeSpecialChars(const std::wstring& line) {
    std::wstring escapedLine;
    for (wchar_t c : line) {
        if (c == L'\\' || c == L'"') {
            escapedLine += L'\\'; 
        }
        escapedLine += c;
    }
    return escapedLine;
}

std::wstring wrapWithSystem(const std::wstring& line) {
    std::wstring escapedLine = escapeSpecialChars(line);
    return L"system(\"" + escapedLine + L"\");";
}

int main() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Batch Files (*.bat)\0*.bat\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        std::wstring inputFilePath(fileName);
        std::wifstream inputFile(inputFilePath);
        if (inputFile) {
            std::wstring outputFilePath = inputFilePath + L"_output.cpp";
            std::wofstream outputFile(outputFilePath);
            if (outputFile) {
                outputFile << "#include <cstdlib>\n\n";
                outputFile << "int main() {\n";
                std::wstring line;
                while (std::getline(inputFile, line)) {
                    if (!line.empty()) {
                        std::wstring wrappedLine = wrapWithSystem(line);
                        outputFile << "    " << wrappedLine << std::endl;
                    }
                }
                outputFile << "    return 0;\n";
                outputFile << "}\n";
                outputFile.close();
                std::wcout << L"Conversion completed. Output file: " << outputFilePath << std::endl;
            }
            else {
                std::cerr << "Error: Unable to open output file." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Unable to open input file." << std::endl;
        }
    }

    return 0;
}
