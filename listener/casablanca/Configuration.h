#pragma once

#include "stdafx.h"
#include "DataSource.h"

namespace Root
{
	namespace BugListener
	{

		namespace Configuration
		{
			using DataSourcesArray = std::vector<DataSource>;
			using DataSourceCollection = std::unordered_map<std::string, DataSourcesArray>;

			class Configuration
			{
				Configuration() {/*empty*/ };
				~Configuration() {/*empty*/ };

			public:
				static Configuration& Instance()
				{
					static Configuration object;
					return object;
				}

			public:

				// 
				void Load(web::json::value&& contents);

				// all configuration entries designated by given API
				const DataSourcesArray& GetDataSources(const std::string& API);

			private:
				DataSourceCollection m_DataSources; // maps API -> to number of entries in config
			};

		}// namespace Configuration

	}// namespace BugListener
}// namespace Root

