#include "stdafx.h"

#include "JiraClient.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			using namespace web;                        // Common features like URIs.
			using namespace web::http;                  // Common HTTP functionality
			using namespace web::http::client;          // HTTP client features

			void JiraClient::RequestIssues(const DataSource& dataSource, SimpleJsonCallback callback)
			{
				assert(dataSource.m_VersionAPI == 1);
#ifdef USED_CXX1z
				constexpr auto requestUri = GET_JIRA_TYPE(JiraType::JiraSession);

#else // USED_CXX1z
				constexpr auto requestUri = GET_JIRA_TYPE("rest/auth/1/session");
#endif

				http_request request{ methods::POST };
				request.set_request_uri(uri{ requestUri });
				auto &headers = request.headers();
				headers.set_content_type(L"application/json");

				json::value body;
#ifdef USED_CXX1z
				body[GET_JIRA_TYPE(JiraType::JiraUsername)] = { m_credentials.username() };
				body[GET_JIRA_TYPE(JiraType::JiraPassword)] = { m_credentials.password() };

#else // USED_CXX1z
				body[GET_JIRA_TYPE("username")] = json::value{ utility::conversions::to_string_t(dataSource.m_UserName) };
				body[GET_JIRA_TYPE("password")] = json::value{ utility::conversions::to_string_t(dataSource.m_UserPassword) };
#endif

				request.set_body(utility::conversions::to_utf8string(body.serialize()), "application/json; charset=utf-8");

				m_http_client = std::make_unique<http_client>(uri(utility::conversions::to_string_t(dataSource.m_URL)));
				auto openSession = m_http_client->request(request).then([&](http_response response)
				{
					return response.extract_json();
				}
				).then([&](json::value body_json) 
				{
					bool success = false;
					if (!body_json[L"session"].is_null())
					{
						const auto name = body_json[L"session"][L"name"];
						const auto value = body_json[L"session"][L"value"];
						m_cache[L"Cookie"] = json::value{ name.as_string() + L"=" + value.as_string() };
						success = true;
					}

					return pplx::task_from_result<bool>(success);
				}
				).then([&](bool success)
				{
					if (success)
					{
						http_request request(methods::GET);
						request.set_request_uri(uri_builder().set_path(L"rest/api/2/search?jql=assignee=currentuser()").to_uri());
						request.headers().add(L"Cookie", m_cache[L"Cookie"]);
						return m_http_client->request(request);
					}

					http_response error;
					error.set_body(json::value());
					return pplx::task_from_result<http_response>(error);
				}
				).then([&](http_response response)
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
					// TODO: implement proper what() encoding, it is not standartized
					//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;
					//std::wostringstream ss;
					//ss << utf16conv.from_bytes(text.c_str()) << std::endl;
					//std::wcout << ss.str();
					std::cout << "Error exception on openSession: " << e.what() << std::endl;
				}
			};

			void JiraClient::GetIssueByKey(const char* const pszKey)
			{
#ifdef USED_CXX1z
				constexpr auto requestUri = GET_JIRA_TYPE(JiraType::JiraIssue);
#else // USED_CXX1z
				constexpr auto requestUri = GET_JIRA_TYPE("rest/api/2/issue/");
#endif
				http_request request{ methods::GET };
				request.set_request_uri(uri{ requestUri + utility::conversions::to_string_t(pszKey) });

			}

		}// namespace Core

	}// namespace BugListener
}// namespace Root
