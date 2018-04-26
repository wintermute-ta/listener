#include "stdafx.h"

#include "RedmineClient.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			using namespace web;                        // Common features like URIs.
			using namespace web::http;                  // Common HTTP functionality
			using namespace web::http::client;          // HTTP client features

			const char* const RedmineClient::pszApiKey = "apikey:";
			const char* const RedmineClient::pszUserID = "userid:";

			void RedmineClient::RequestIssues(const DataSource& dataSource, SimpleJsonCallback callback)
			{
				assert(dataSource.m_VersionAPI == 1);

				credentials cred(utility::conversions::to_string_t(dataSource.m_UserName), utility::conversions::to_string_t(dataSource.m_UserPassword));
				if (!cred.is_set())
				{
					// must use API KEY if not setting username / password set
					assert(dataSource.m_Metadata.find(pszApiKey) != std::string::npos);
					std::string apiKey = dataSource.m_Metadata.substr(strlen(pszApiKey));
				}

				utility::stringstream_t request_uri;
				request_uri << L"issues.json?assigned_to_id=";
				if (dataSource.m_Metadata.find(pszUserID) != std::string::npos)
				{
					request_uri << utility::conversions::to_string_t(dataSource.m_Metadata.substr(strlen(pszUserID)));
				}
				else
				{
					request_uri << L"me";
				}

				http_request request{ methods::GET };
				request.set_request_uri(request_uri.str());
				request.headers().set_content_type(L"application/json");

				uri client_uri(uri(utility::conversions::to_string_t(dataSource.m_URL)));
				if (cred.is_set())
				{
					http_client_config client_config;
					client_config.set_credentials(cred);
					m_http_client = std::make_unique<http_client>(client_uri, client_config);
				}
				else
				{
					m_http_client = std::make_unique<http_client>(client_uri);
				}

				auto openSession = m_http_client->request(request).then([&](http_response response)
				{
					return response.extract_json();
				}
				).then([&](json::value body_json)
				{
					callback(body_json);
				});

				try
				{
					openSession.wait();
				}
				catch (const std::exception &e)
				{
					printf("Error exception:%s\n", e.what());
				}
			}

			void RedmineClient::CreateIssue(const DataSource& dataSource, const std::string& strDescription, int nPriority)
			{
				assert(dataSource.m_VersionAPI == 1);

				credentials cred(utility::conversions::to_string_t(dataSource.m_UserName), utility::conversions::to_string_t(dataSource.m_UserPassword));
				if (!cred.is_set())
				{
					// must use API KEY if not setting username / password
					assert(dataSource.m_Metadata.find(pszApiKey) != std::string::npos); 
					std::string apiKey = dataSource.m_Metadata.substr(strlen(pszApiKey));
				}

				http_request request{ methods::POST };
				request.set_request_uri(L"issues.json");
				request.headers().set_content_type(L"application/json");

				json::value body;
				body[L"issue"][L"project_id"] = json::value(6); // TR-Casablanca 
				body[L"issue"][L"subject"] = json::value(utility::conversions::to_string_t(strDescription));
				body[L"issue"][L"priority_id"] = json::value(nPriority);
				request.set_body(utility::conversions::to_utf8string(body.serialize()), utf8string{ "application/json; charset=utf-8" });

				uri client_uri(uri(utility::conversions::to_string_t(dataSource.m_URL)));
				if (cred.is_set())
				{
					http_client_config client_config;
					client_config.set_credentials(cred);
					m_http_client = std::make_unique<http_client>(client_uri, client_config);
				}
				else
				{
					m_http_client = std::make_unique<http_client>(client_uri);
				}

				auto openSession = m_http_client->request(request).then([&](http_response response)
				{
					return response.extract_json();
				}
				).then([&](json::value body_json)
				{
					std::wcout << body_json.serialize() << std::endl; // "Created" 
				});

				try
				{
					openSession.wait();
				}
				catch (const std::exception &e)
				{
					printf("Error exception:%s\n", e.what());
				}
			}

		}// namespace Core

	}// namespace BugListener
}// namespace Root