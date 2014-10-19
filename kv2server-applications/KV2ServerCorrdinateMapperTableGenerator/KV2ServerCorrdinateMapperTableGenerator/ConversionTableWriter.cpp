#include "ConversionTableWriter.h"

#include <iostream>
#include <fstream>

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}

ConversionTableWriter::ConversionTableWriter(void)
	: kinectSensor(NULL)
	, coordinateMapper(NULL)
	, conversionTable(NULL)
{
	//conversionTable = new PointF[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];

	HRESULT hr = GetDefaultKinectSensor(&kinectSensor);

	if (FAILED(hr))
	{
		std::cout << "FAILED: GetDefaultKinectSensor()!\n"; 
		return;
	}

	hr = kinectSensor->Open();

	if (SUCCEEDED(hr))
	{
		hr = kinectSensor->get_CoordinateMapper(&coordinateMapper);
	} else std::cout << "FAILED: check if Kinect is properly connected!\n";

	UINT32 tableEntryCount = K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT;

	int numTry = 0;
	while (conversionTable == NULL || conversionTable[0].Y < 0)
	{
		Sleep(100);
		numTry++;
		hr = coordinateMapper->GetDepthFrameToCameraSpaceTable(&tableEntryCount, &conversionTable);
	}
	std::cout << "\nSuccessfully retrieved a valid conversion table after trying " << numTry << " times...\n\n";

	SafeRelease(coordinateMapper);
}

ConversionTableWriter::~ConversionTableWriter(void)
{
	// close the Kinect Sensor
    if (kinectSensor)
    {
        kinectSensor->Close();
    }
    SafeRelease(kinectSensor);
}

void ConversionTableWriter::WriteTableToBinaryFile(std::string filename)
{
	std::cout << "\nWriting to binary file...\n\n";

	std::ofstream outputStream(filename, std::ios::out | std::ios::binary);
	
	outputStream.write((char*) conversionTable, K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT * sizeof(PointF));
	outputStream.flush();
	outputStream.close();

	DWORD bufferLength			= GetCurrentDirectory(0, NULL);
	LPWSTR  currentDirectory	= new TCHAR[bufferLength];
	GetCurrentDirectory(bufferLength, currentDirectory);

	std::cout << "Conversion table written at directory:\n";
	std::wcout << currentDirectory << std::endl << std::endl;
	std::cout << "with filename: " << filename << "\n\n"; 
}

void ConversionTableWriter::ValidateBinaryFile(std::string filename)
{
	std::ifstream inputStream(filename, std::ios::in | std::ios::binary);
	
	PointF* temp = new PointF[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];

	inputStream.read((char*) temp, K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT * sizeof(PointF));
	inputStream.close();

	// validate

	const int tableEntryCount = K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT;

	for (int i = 0; i < tableEntryCount; i++)
	{
		if (!(conversionTable[i].X == temp[i].X && conversionTable[i].Y == temp[i].Y))
		{
			std::cout << "ERROR!\n";
		}
	}

}


void ConversionTableWriter::PrintTable()
{
	if (conversionTable == NULL) return;

	const int tableEntryCount = K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT;

	for (int i = 0; i < tableEntryCount; i+=10000)
	{
		std::cout << conversionTable[i].X << " ";
		std::cout << conversionTable[i].Y;
		std::cout << std::endl;
	}
}

void ConversionTableWriter::RetrieveUniqueKinectId()
{
	UINT	bufferSize = 1000;
	LPWSTR	uniqueKinectId = new WCHAR[bufferSize];

	kinectSensor->get_UniqueKinectId(bufferSize, uniqueKinectId);

	std::wcout << "id:" << uniqueKinectId << "_end_" << std::endl;

}