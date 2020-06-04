
#include "RequestHandler.h"
#include "json.h"
#include<fstream>
#include <string>
#include <iostream>
#include<vector>
#include <filesystem>
namespace fs = std::filesystem;   //C++ 17
using std::vector;
using std::string;

#import "D:\Practice\DocProcessor\DocProcessor\bin\Debug\DocProcessor.tlb"  no_namespace //add the correct path of tlb

void writetoTemp(string str)
{
	std::ofstream outf;
	outf.open("D:\\nativehost.txt", std::ios::out | std::ios::app);
	outf << str << std::endl;
	outf.close();
}

/*
vector<string> get_all_files_names_within_folder(string folder)

This function gets all the files present in given folder.
Returns the vector containing file list
*/
vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> files;
	for (const auto& entry : fs::directory_iterator(folder))
		files.push_back(entry.path().filename().string());
	return files;
}

/*
string form_json_of_files1(string directory)

This function forms json string which outputs list of files
*/
string form_json_of_files1(string directory)
{
	string response = "{\"files\" : [";
	vector<string> files = get_all_files_names_within_folder(directory);
	if (files.size() > 0)
	{
		string firstFile = files[0];
		response += "\"" + firstFile + "\"";
		for (unsigned int i = 1; i < files.size(); i++)
		{
			response += ",\"" + files[i] + "\"";
		}
	}
	response += "]}";
	return response;
}

/*
// for C++ versions earlier than C++ 17

vector<wstring> get_all_files_names_within_folder(wstring folder)
{
	vector<std::wstring> names;
	wstring search_path = folder + L"/*.docx";
	writetoTemp(std::string(search_path.begin(), search_path.end()));
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

string form_json_of_files(string directory)
{
	std::wstring wDirectory(directory.length(), L' '); // Make room for characters
	std::copy(directory.begin(), directory.end(), wDirectory.begin());

	vector<std::wstring> files = get_all_files_names_within_folder(wDirectory);
	writetoTemp("Total files = " + to_string(files.size()));

	string response = "{\"files\" : [";
	if (files.size() > 0)
	{
		wstring firstFile = files[0];
		response += "\"" + string(firstFile.begin(), firstFile.end()) + "\"";
		for (unsigned int i = 1; i < files.size(); i++)
		{
			wstring str = files[i];
			std::string s(str.begin(), str.end());
			writetoTemp(s);
			response += ",\"" + s + "\"";
		}
	}
	response += "]}";
	return response;
}

*/

/*
std::string  processRequest(char* jsonMsg)

This function processes the request received from extension.

There can be two requests

"request" : "GetFileList" -> to get the list of files from a configured directory
"request" : "addText" -> to add the selected text in selected file.
*/
std::string  processRequest(char* jsonMsg)
{
	std::string response = "";
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(jsonMsg, root);
	if (!parsingSuccessful)
	{
		return response;
	}

	std::string request = root.get("request", "").asString();

	//writetoTemp(request);
	if (request == "GetFileList")
	{
		string directory = root.get("directory", "").asString();

		response = form_json_of_files1(directory); //using C++ 17 feature
		//response = form_json_of_files(directory);  // below C++ 17

		//writetoTemp(response);
	}
	if (request == "addText")
	{
		response = "{\"addedtext\" : ";
		std::string filepath = root.get("filepath", "").asString();
		std::string filename = root.get("filename", "").asString();
		std::string text = root.get("text", "").asString();
		//writetoTemp("Filepath  = " + filepath);
		//writetoTemp("Filename = " + filename);
		//writetoTemp("Text copied = " + text);
		VARIANT_BOOL isNewFile = VARIANT_FALSE;
		if (filename == "Write a new file")
			isNewFile = VARIANT_TRUE;
		try
		{
			CoInitialize(NULL);
			bool ret = false;
			IDOCProcessorPtr ptr; 
			HRESULT hr = ptr.CreateInstance(__uuidof(DocProcessor));  // COM instance that will add text to docx files using Microsoft.Office library.
			if (SUCCEEDED(hr))
			{
				ret = ptr->AddDataToFile(_bstr_t(filepath.c_str()), _bstr_t(filename.c_str()), _bstr_t(text.c_str()), isNewFile);

				if (ret)
				{
					//writetoTemp("ret is true");
					response += "\"true\"}";
				}
				else
				{
					//writetoTemp("ret is false;");
					response += "\"false\"}";
				}

			}
			else
			{
				//writetoTemp("Failed");
				response += "\"false\"}";
			}

			CoUninitialize();
		}
		catch (...)
		{
			response += "\"false\"}";
			//writetoTemp("Exception...");
		}

		//writetoTemp("Response = " + response);
	}
	return response;
}

