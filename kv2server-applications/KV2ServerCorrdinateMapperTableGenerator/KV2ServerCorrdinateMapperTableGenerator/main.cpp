#include <iostream>

#include "ConversionTableWriter.h"

int main(int argc, char** argv)
{
	std::string filename;

	std::cout << "This application will generate a .kv2dat binary which contains the conversion table retrieved from a specific Kinect V2 sensor.\n\n";
	std::cout << "Please enter the name of your table WITHOUT the .kv2dat extension (e.g. KV2SENSOR01 ):\n";
	std::cin >> filename;

	filename += ".kv2dat";

	ConversionTableWriter writer;

	//writer.RetrieveUniqueKinectId();
	//writer.PrintTable();
	writer.WriteTableToBinaryFile(filename);
	writer.ValidateBinaryFile(filename);

	std::cout << "Reached end of program: Press Ctrl+C to quit.\n";
	while(1);
	return 0;
}