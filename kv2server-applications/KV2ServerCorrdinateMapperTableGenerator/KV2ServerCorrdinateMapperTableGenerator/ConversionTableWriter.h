#pragma once

#include <string>
#include "Kinect.h" 

#define K4W2_DEPTH_WIDTH	512
#define K4W2_DEPTH_HEIGHT	424

class ConversionTableWriter
{
public:

	ConversionTableWriter(void);
	~ConversionTableWriter(void);

	void PrintTable();
	void RetrieveUniqueKinectId();

	void WriteTableToBinaryFile(std::string output_filename);
	void ValidateBinaryFile(std::string filename);

private:

	IKinectSensor*		kinectSensor;
	ICoordinateMapper*	coordinateMapper;

	PointF*				conversionTable;
};

