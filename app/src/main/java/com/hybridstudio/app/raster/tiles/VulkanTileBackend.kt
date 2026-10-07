package com.hybridstudio.app.raster.tiles

import android.util.Log

class VulkanTileBackend {

    private var isVulkanReady = false

    companion object {
        init {
            try {
                System.loadLibrary("vulkan_tile_backend")
            } catch (e: UnsatisfiedLinkError) {
                Log.e("VulkanBackend", "Error cargando librería nativa", e)
            }
        }
    }

    external fun initVulkan(): Boolean
    external fun processTileNative(pixelData: IntArray, width: Int, height: Int, opType: Int): Boolean
    external fun cleanupVulkan()

    fun initialize(): Boolean {
        isVulkanReady = try {
            initVulkan()
        } catch (e: Exception) {
            false
        }
        return isVulkanReady
    }

    fun processTile(pixelData: IntArray, width: Int, height: Int, opType: Int): Boolean {
        if (!isVulkanReady) return false
        return try {
            processTileNative(pixelData, width, height, opType)
        } catch (e: Exception) {
            isVulkanReady = false
            false
        }
    }

    fun close() {
        if (isVulkanReady) {
            cleanupVulkan()
            isVulkanReady = false
        }
    }
}
