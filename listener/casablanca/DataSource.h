#pragma once

#include "stdafx.h"

namespace Root
{
	namespace BugListener
	{

		namespace Configuration
		{
			struct DataSource
			{
				std::string	m_URL;
				int			m_VersionAPI;
				std::string	m_UserName;
				std::string	m_UserPassword;
				std::string	m_Metadata;
			};

		}// namespace Configuration

	}// namespace BugListener
}// namespace Root