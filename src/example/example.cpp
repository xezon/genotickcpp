
#include <igenotick.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

template<typename T, unsigned int size>
unsigned int GetArrayLength(T(&)[size])
{
	return size;
}

template<int size>
void SetString(TGenotickString& dst, char(&src)[size])
{
	dst.utf8_buffer = src;
	dst.capacity = size;
}

void SetConstString(TGenotickString& dst, const char* src)
{
	dst.utf8_buffer = const_cast<char*>(src);
	dst.capacity = 0;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = nullptr;
	TGenotickJvmSettings jvmSettings = { 0 };
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == EGenotickResult::Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion();

		TGenotickMainSettings mainSettings;
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		SetString(mainSettings.populationDAO, buffer1);
		SetString(mainSettings.dataDirectory, buffer2);

		result = pInstance->GetSettings(&mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		SetConstString(mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(&mainSettings);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		TGenotickStartSettings startSettings = { 0 };
		startSettings.parameters = arguments;
		startSettings.parameterCount = GetArrayLength(arguments);

		result = pInstance->Start(&startSettings);

		GenotickSafeRelease(pInstance);
	}

	return 0;
}