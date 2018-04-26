#pragma once
#include "stdafx.h"

#include "DataSource.h"

namespace Root
{
	namespace BugListener
	{
		namespace Core
		{
			using SimpleJsonCallback = std::function<void(web::json::value)>;
			using DataSource = Root::BugListener::Configuration::DataSource;

			class HttpClient
			{
			public:
				HttpClient();
				virtual ~HttpClient();

			protected:

				/* HTTP client class, used to maintain a connection to an HTTP service for an extended session */
				std::unique_ptr<web::http::client::http_client>	m_http_client;	
			};

		}// namespace Core

	}// namespace BugListener
}// namespace Root
