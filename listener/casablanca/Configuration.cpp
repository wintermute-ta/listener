#include "stdafx.h"

#include "Configuration.h"

namespace Root
{
	namespace BugListener
	{

		namespace Configuration
		{
			using namespace std;
			using namespace web;

			void Configuration::Load(json::value&& contents)
			{
				const json::value dataSources = contents[L"Trackers"];
				for (size_t index = 0; index < dataSources.size(); ++index)
				{
					string API = utility::conversions::to_utf8string( dataSources.at(index).at(L"API").as_string() );

					DataSource dataSource;
					dataSource.m_URL = utility::conversions::to_utf8string(dataSources.at(index).at(L"URL").as_string());
					dataSource.m_VersionAPI = dataSources.at(index).at(L"version").as_integer();
					dataSource.m_UserName = utility::conversions::to_utf8string(dataSources.at(index).at(L"login").as_string());
					dataSource.m_UserPassword = utility::conversions::to_utf8string(dataSources.at(index).at(L"password").as_string());
					dataSource.m_Metadata = utility::conversions::to_utf8string(dataSources.at(index).at(L"meta").as_string());

					m_DataSources[API].push_back(dataSource);
				}
			}

			const DataSourcesArray& Configuration::GetDataSources(const string& API)
			{
				auto pair = m_DataSources.find(API);
				if (pair == m_DataSources.end())
				{
					static DataSourcesArray blank;
					return blank;
				}

				return pair->second;
			}

		}// namespace Configuration

	}// namespace BugListener
}// namespace Root