#pragma once
#include "stdafx.h"

#include "TrackerItem.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			using TrackerItemCollection = std::list<TrackerItem>;
			using RawItemCollection = concurrency::concurrent_vector<web::json::value>;

			class Core
			{
			public:
				Core() = default;
				~Core() = default;

			public:
				void ReadAllItems(); // syncronous operation
				const TrackerItemCollection& GetTrackerItems() const { return m_TrackerItems; }

			private:
				void _TranslateIssuesToItems(const RawItemCollection& itemsFetched);

			private:
				TrackerItemCollection m_TrackerItems;
			};

		}// namespace Core

	}// namespace BugListener
}// namespace Root


