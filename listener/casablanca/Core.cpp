#include "stdafx.h"

#include "Core.h"

#include "Configuration.h"
#include "JiraClient.h"
#include "RedmineClient.h"
#include "TrackerItem.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			using Configuration = Root::BugListener::Configuration::Configuration;
			using DataSource = Root::BugListener::Configuration::DataSource;
			using DataSourcesArray = Root::BugListener::Configuration::DataSourcesArray;

			void Core::ReadAllItems()
			{
				RawItemCollection issuesFound;
				Concurrency::parallel_for(0, 1, [&](int i)
				{
					switch (i)
					{
						case JIRA_API.index():
						{
							const DataSourcesArray& jiraSources = Configuration::Instance().GetDataSources(JIRA_API.name());
							for (const DataSource& ds : jiraSources)
							{
								JiraClient jc;
								jc.RequestIssues(ds, [&](web::json::value response_json)
								{
									response_json[L"API"] = web::json::value(JIRA_API.index());
									response_json[L"BASE_URL"] = web::json::value(utility::conversions::to_string_t(ds.m_URL));
									issuesFound.push_back(response_json);

#ifdef NEED_JIRA_FORMAT_DEBUG
									std::wcout << utility::conversions::to_string_t(JIRA_API.name()) << " issues: ";
									std::wcout << response_json.serialize() << std::endl; // TODO: autolock the cout, it is not thread safe

									utility::ofstream_t file("jira_json.json");
									response_json.serialize(file);
#endif
								});
							}
						}

						case REDMINE_API.index():
						{
							const DataSourcesArray& jiraSources = Configuration::Instance().GetDataSources(REDMINE_API.name());
							for (const DataSource& ds : jiraSources)
							{
								RedmineClient rc;
								rc.RequestIssues(ds, [&](web::json::value response_json)
								{
									response_json[L"API"] = web::json::value(REDMINE_API.index());
									response_json[L"BASE_URL"] = web::json::value(utility::conversions::to_string_t(ds.m_URL));
									issuesFound.push_back(response_json);

#ifdef NEED_REDMINE_FORMAT_DEBUG
									std::wcout << utility::conversions::to_string_t(REDMINE_API.name()) << " issues: ";
									std::wcout << response_json.serialize() << std::endl; // TODO: autolock the cout, it is not thread safe
#endif
								});
							}
						}
						break;

						default:
							assert(false); // not supported API
					} // switch
				}); // parallel_for

				_TranslateIssuesToItems(issuesFound);
			}

			void Core::_TranslateIssuesToItems(const RawItemCollection& itemsFetched)
			{
				for (web::json::value allItems: itemsFetched)
				{
					const int api = allItems[L"API"].as_integer();

					if (IS_REDMINE_API(api))
					{
						const int count = allItems.has_field(L"total_count") ? allItems[L"total_count"].as_integer() : 0; // TODO: exception
						for (int i = 0; i < count; ++i)
						{
							TrackerItem ti;
							ti.API = REDMINE_API.name();
							const int issue_id = allItems[L"issues"][i][L"id"].as_integer(); // TODO: exception;

							std::wostringstream id;
							id << allItems[L"issues"][i][L"project"][L"name"].as_string() << L"-"; // TODO: exception
							id << issue_id;
							ti.ID = id.str();

							ti.Text = allItems[L"issues"][i][L"subject"].as_string(); // TODO: exception

							std::ostringstream url; // TODO: use uri_builder class here!
							url << utility::conversions::to_utf8string(allItems[L"BASE_URL"].as_string()); // TODO: exception
							url << "/issues/";
							url << issue_id;
							ti.SourceURL = url.str();

							m_TrackerItems.push_back(ti);
						}
					}
					else if (IS_JIRA_API(api))
					{
						const int count = allItems.has_field(L"total") ? allItems[L"total"].as_integer() : 0; // TODO: exception
						for (int i = 0; i < count; ++i)
						{
							TrackerItem ti;
							ti.API = JIRA_API.name();

							ti.ID = allItems[L"issues"][i][L"key"].as_string(); // TODO: exception
							ti.Text = allItems[L"issues"][i][L"fields"][L"description"].as_string(); // TODO: exception

							std::ostringstream url; // TODO: use uri_builder class here!
							url << utility::conversions::to_utf8string(allItems[L"BASE_URL"].as_string()); // TODO: exception
							url << "/browse/";
							url << utility::conversions::to_utf8string(ti.ID); // TODO: exception
							ti.SourceURL = url.str();

							m_TrackerItems.push_back(ti);
						}
					}
					else
					{
						assert(false); // unsupported API
					}
				}
			}

		}// namespace Core

	}// namespace BugListener
}// namespace Root
