
#include <igenotick.h>
#include <igenotick_utils.h>
#include <assert.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

void GenerateDataPoints(TGenotickDataPoint* dataPoints, TGenotickSize size)
{
	for (TGenotickSize i = 0; i < size; ++i)
	{
		dataPoints[i].time   = 20130101 + i;
		dataPoints[i].open   = size*3 + i * 1.0;
		dataPoints[i].high   = size*3 + i * 1.0 + 0.5;
		dataPoints[i].low    = size*3 + i * 1.0 - 0.5;
		dataPoints[i].close  = size*3 + i * 1.0 + 0.2;
		dataPoints[i].volume = 10000.0;
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	EGenotickResult result;

	IGenotick* pInstance = 0;
	TGenotickLoadSettings creationSettings = { 0 };
	creationSettings.utf8_jvmDllPath = JVM_PATH;
	creationSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	result = GenotickCreate(&pInstance, &creationSettings);
	assert(result == GenotickResult_Success);

	IGenotickList* pInstances = 0;

	result = GenotickGetInstances(&pInstances, &creationSettings);
	assert(result == GenotickResult_Success);

	GENOTICK_SAFE_RELEASE(pInstances);

	TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);
	assert(version == GENOTICK_INTERFACE_VERSION);

	TGenotickInt32 sessionId = 0;
	result = pInstance->CreateSession(pInstance, sessionId);
	assert(result == GenotickResult_Success);

	TGenotickMainSettings mainSettings = { 0 };
	char buffer1[260] = { 0 };
	char buffer2[260] = { 0 };
	GenotickSetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
	GenotickSetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

	result = pInstance->GetSettings(pInstance, sessionId, &mainSettings);
	assert(result == GenotickResult_Success);

	mainSettings.startTimePoint = 20130101;
	mainSettings.endTimePoint = 20150101;
	GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

	result = pInstance->ChangeSettings(pInstance, sessionId, &mainSettings);
	assert(result == GenotickResult_Success);

	TGenotickDataPoint dataPoints[200] = { 0 };
	TGenotickAssetData assetData = { 0 };
	assetData.assetName = "ASSET";
	assetData.dataPoints = dataPoints;
	assetData.dataPointCount = GENOTICK_ARRAY_SIZE(dataPoints);
	assetData.firstDataPointIsNewest = GenotickFalse;
	GenerateDataPoints(dataPoints, assetData.dataPointCount);

	result = pInstance->SetAssetData(pInstance, sessionId, &assetData);
	assert(result == GenotickResult_Success);

	const char* arguments[] =
	{
		"input=external",
		"outdir=" GENOTICK_OUTDIR,
	};
	TGenotickStartArgs startArgs = { 0 };
	startArgs.elements = arguments;
	startArgs.elementCount = GENOTICK_ARRAY_SIZE(arguments);

	result = pInstance->Start(pInstance, sessionId, &startArgs);
	assert(result == GenotickResult_Success);

	IGenotickTimePoints* pTimePoints = 0;
	IGenotickPredictions* pPredictions = 0;
	TGenotickTimePoint timePoint = 0;
	EGenotickPrediction prediction = GenotickPrediction_Out;

	result = pInstance->GetTimePoints(pInstance, sessionId, &pTimePoints);
	assert(result == GenotickResult_Success);

	result = pInstance->GetPredictions(pInstance, sessionId, "ASSET", &pPredictions);
	assert(result == GenotickResult_Success);

	result = pInstance->GetNewestTimePoint(pInstance, sessionId, &timePoint);
	assert(result == GenotickResult_Success);

	result = pInstance->GetNewestPrediction(pInstance, sessionId, "ASSET", &prediction);
	assert(result == GenotickResult_Success);

	result = pInstance->RemoveSession(pInstance, sessionId);
	assert(result == GenotickResult_Success);

	GENOTICK_SAFE_RELEASE(pTimePoints);
	GENOTICK_SAFE_RELEASE(pPredictions);
	GENOTICK_SAFE_RELEASE(pInstance);

	return 0;
}
