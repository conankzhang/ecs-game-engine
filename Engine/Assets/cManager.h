/*
	An asset manager is in charge of a specific asset type:
		* The manager works like a factory when an asset is loaded
			and ensures that a single asset is only loaded once even if multiple load requests are made
		* The manager tracks handles for assets,
			and can return the asset's actual pointer given its handle
		* When every handle to an asset has been released
			the manager releases its own reference to the asset so that it can be unloaded
*/

#ifndef EAE6320_ASSETS_CMANAGER_H
#define EAE6320_ASSETS_CMANAGER_H

// Includes
//=========

#include "cHandle.h"

#include <cstdint>
#include <Engine/Concurrency/cMutex.h>
#include <Engine/Results/Results.h>
#include <map>
#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Assets
	{
			template <class tAsset>
		class cManager
		{
			// Interface
			//==========

		public:

			// Access
			//-------

			// This function returns the actual pointer to the asset associated with the handle
			// or NULL if the handle doesn't point to a valid asset
			tAsset* Get( const cHandle<tAsset> i_handle );

			// Every handle returned from a successful call to Load() with a given path
			// must be passed to Release() when the caller is finished with it
				template <typename... tConstructorArguments>
			cResult Load( const char* const i_path, cHandle<tAsset>& o_handle, tConstructorArguments&&... i_constructorArguments );
			cResult Release( cHandle<tAsset>& io_handle );

			// Initialization / Clean Up
			//--------------------------

			cResult Initialize();
			cResult CleanUp();

			~cManager<tAsset>();

			// Data
			//=====

		private:

			struct sAssetRecord
			{
				tAsset* asset = nullptr;
				uint16_t id = cHandle<tAsset>::InvalidValue;
				uint16_t referenceCount = 0;

				sAssetRecord( tAsset* const i_asset, const uint16_t i_id, const uint16_t i_referenceCount );
			};
			std::vector<sAssetRecord> m_assetRecords;
			std::vector<uint16_t> m_unusedAssetRecordIndices;
			std::map< std::string, cHandle<tAsset> > m_map_pathsToHandles;
			eae6320::Concurrency::cMutex m_mutex;
		};
	}
}

#include "cManager.hpp"

#endif	// EAE6320_ASSETS_CMANAGER_H
