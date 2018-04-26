#pragma once

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			class SupportedAPI
			{

			public:
				enum class API
				{
					JIRA = 0,
					REDMINE = 1
				};

				template<API>
				struct impl { };

				template<>
				struct impl<API::JIRA>
				{
					constexpr int index() const { return 0; }
					const char* const name() const { return "JIRA";  }
					
				};

				template<>
				struct impl<API::REDMINE>
				{
					constexpr int index() const { return 1; }
					const char* const name() const { return "REDMINE"; }
				};

			public:
				SupportedAPI() = delete;
				~SupportedAPI() = delete;
			};

			// TODO: extern these and create .cpp file to instantiate once!
			const SupportedAPI::impl<SupportedAPI::API::JIRA> JIRA_API;
			const SupportedAPI::impl<SupportedAPI::API::REDMINE> REDMINE_API;

			inline bool IS_REDMINE_API(int index)				{ return REDMINE_API.index() == index;	}
			inline bool IS_JIRA_API(int index)					{ return JIRA_API.index() == index;		}
			inline bool IS_REDMINE_API(const char* const psz)	{ return 0 == strcmp( REDMINE_API.name(), psz); }
			inline bool IS_JIRA_API(const char* const psz)		{ return 0 == strcmp(JIRA_API.name(), psz); }


		}// namespace Core

	}// namespace BugListener
}// namespace Root

