#pragma once
#include "stdafx.h"

#include "HttpClient.h"
#include "DataSource.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			enum class JiraType
			{
				JiraUsername,
				JiraPassword,
				JiraSession,
				JiraIssue
			};

			template<class T>
			using JiraTypeHandler = std::pair<JiraType, T>;

			template<class ... Vector>
			struct JiraMap
			{
				constexpr JiraMap(JiraTypeHandler<Vector> ... JRH)
					: indexes{ JRH.first... }, vectors{std::move(JRH.second)...}
				{}

				constexpr std::size_t GetIndex(JiraType i) const
				{
					for (size_t indx = 0; indx < indexes.size(); indx++)
					{
						if (i == indexes[indx])
						{
							return indx;
						}
					}

					return std::numeric_limits<std::size_t>::max();
				}

				std::array<JiraType,   sizeof...(Vector)> indexes;
				std::tuple<Vector...> vectors;
			};

			template <class ... T>
			constexpr auto make_jira_map(JiraTypeHandler<T> ... t)
			{
				return JiraMap<T...>{std::move(t)...};
			}

			template <class T>
			constexpr auto make_jira_type_handler(JiraType jrt, T t)
			{
				return JiraTypeHandler<T>{jrt, std::move(t)};
			}

			constexpr auto jiraRequestMap = make_jira_map(
				make_jira_type_handler(
					JiraType::JiraUsername,
					L"username"
				),
				make_jira_type_handler(
					JiraType::JiraPassword,
					L"password"
				),
				make_jira_type_handler(
					JiraType::JiraSession,
					L"rest/auth/1/session"
				),
				make_jira_type_handler(
					JiraType::JiraIssue,
					L"rest/api/2/issue/"
				)
			);
			#ifdef USED_CXX1z
			#define GET_JIRA_TYPE(JIRA_TYPE) std::get<jiraRequestMap.GetIndex((JIRA_TYPE))>(jiraRequestMap.vectors)
			#else // USED_CXX1z
				#define GET_JIRA_TYPE(JIRA_URL) U(JIRA_URL)
			#endif

			using DataSource = Root::BugListener::Configuration::DataSource;

			class JiraClient : public HttpClient
			{
			public:
				JiraClient() = default;

				void RequestIssues(const DataSource& dataSource, SimpleJsonCallback callback);
				void GetIssueByKey(const char* const pszKey);

			private:
				web::json::value				m_cache;		/* caching JSESSIONID*/
			};

		}// namespace Core

	}// namespace BugListener
}// namespace Root