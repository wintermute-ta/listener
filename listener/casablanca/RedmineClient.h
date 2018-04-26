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

			class RedmineClient : public HttpClient
			{
			public:
				RedmineClient() = default;

				void RequestIssues(const DataSource& ds, SimpleJsonCallback callback);
				void CreateIssue(const DataSource& ds, const std:: string& strDescription, int nPriority);

			private:
				static const char* const pszApiKey;
				static const char* const pszUserID;
			};

		}// namespace Core

	}// namespace BugListener
}// namespace Root
