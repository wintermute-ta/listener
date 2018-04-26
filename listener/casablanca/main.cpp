#include "stdafx.h"

#include "Core.h"
#include "Configuration.h"
#include "Vizualizer.h"

using Configuration = Root::BugListener::Configuration::Configuration;
using Core = Root::BugListener::Core::Core;
using Visualizer = Root::BugListener::Visualization::Vizualizer;

int main(int argc, char* argv[])
{
	try
	{
		utility::stringstream_t configStream;
		configStream << utility::ifstream_t(argc == 2 ? argv[1] : "config.json").rdbuf();

		Configuration::Instance().Load(web::json::value::parse(configStream));

		Core core;
		core.ReadAllItems();

		Visualizer::Visualize(core.GetTrackerItems());
		return 0;
	}
	catch (const web::json::json_exception& e)
	{
		std::cout << "Error malformed JSON: " << e.what() << std::endl;
		return 1;
	}
}