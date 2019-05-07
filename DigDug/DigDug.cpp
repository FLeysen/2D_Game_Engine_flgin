#include "pch.h"
#pragma comment(lib,"xinput.lib")
#include <vld.h>
#include "SDL.h"
#include "Flgin.h"
#include "TextLocalizer.h"

#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) 
{
#pragma warning( pop )
	FTextLocalizer.Load("loca.csv", "en-uk");
	flgin::Flgin engine{};
	engine.Run();
	return 0;
}
