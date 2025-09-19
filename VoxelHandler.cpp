#include "VoxelHandler.h"
#include "Log.h"
#include "Utils.h"

#include <imgui/imgui.h>
#include <cmath>

#include "Player.h"

VoxelHandler::VoxelHandler(Camera* camera)
	: m_Camera(camera)
{
	LOG_INFO("VOXELHANDLER");
}

void VoxelHandler::UpdateChunks(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* chunks) {
	VoxelHandler::m_Chunks = chunks;
}

void VoxelHandler::RayCasting() {
	float maxX, maxY, maxZ, deltaX, deltaY, deltaZ;

	float x1 = m_Camera->Position.x;
	float y1 = m_Camera->Position.y;
	float z1 = m_Camera->Position.z;

	ImGui::Text((m_Camera->GetCameraPosition()).c_str());
	//ImGui::Text((m_Camera->GetCameraOrientation()).c_str());

	if (std::roundf(m_Camera->Front.x) == 1) {
		ImGui::Text("Facing Est");
	}
	else if (std::roundf(m_Camera->Front.x) == -1) {
		ImGui::Text("Facing West");
	}
	else if (std::roundf(m_Camera->Front.z) == 1) {
		ImGui::Text("Facing North");
	}
	else if (std::roundf(m_Camera->Front.z) == -1) {
		ImGui::Text("Facing South");
	}

	ImGui::Text(("Look at: " + glm::to_string(m_VoxelWorldPos)).c_str());

	auto normOri = glm::normalize(m_Camera->Front);

	float x2 = x1 + m_Camera->Front.x * MAX_RAY_DIST;
	float y2 = y1 + m_Camera->Front.y * MAX_RAY_DIST;
	float z2 = z1 + m_Camera->Front.z * MAX_RAY_DIST;

	glm::vec3 currentVoxelPos = m_Camera->Position;
	m_VoxelId = 0;
	m_VoxelNormal = glm::vec3(0);
	int stepDir = -1;

	int dx = glm::sign(x2 - x1);
	if (dx != 0) deltaX = std::min(dx / (x2 - x1), 10000000.0f); else deltaX = 10000000.0f;
	if (dx > 0) maxX = deltaX * (1.0f - glm::fract(x1)); else maxX = deltaX * glm::fract(x1);

	int dy = glm::sign(y2 - y1);
	if (dy != 0) deltaY = std::min(dy / (y2 - y1), 10000000.0f); else deltaY = 10000000.0f;
	if (dy > 0) maxY = deltaY * (1.0f - glm::fract(y1)); else maxY = deltaY * glm::fract(y1);

	int dz = glm::sign(z2 - z1);
	if (dz != 0) deltaZ = std::min(dz / (z2 - z1), 10000000.0f); else deltaZ = 10000000.0f;
	if (dz > 0) maxZ = deltaZ * (1.0f - glm::fract(z1)); else maxZ = deltaZ * glm::fract(z1);

	while (not(maxX > 1.0f && maxY > 1.0f && maxZ > 1.0f)) {
		m_VoxelId = GetHitVoxelId(currentVoxelPos);

		if (m_VoxelId > 0) {
			m_VoxelWorldPos = currentVoxelPos;

			if (stepDir == 0) {
				m_VoxelNormal.x = -dx;
			}
			else if (stepDir == 1) {
				m_VoxelNormal.y = -dy;
			}
			else {
				m_VoxelNormal.z = -dz;
			}
			return;
		}

		if (maxX < maxY) {
			if (maxX < maxZ) {
				currentVoxelPos.x += dx;
				maxX += deltaX;
				stepDir = 0;
			}
			else {
				currentVoxelPos.z += dz;
				maxZ += deltaZ;
				stepDir = 2;
			}
		}
		else {
			if (maxY < maxZ) {
				currentVoxelPos.y += dy;
				maxY += deltaY;
				stepDir = 1;
			}
			else {
				currentVoxelPos.z += dz;
				maxZ += deltaZ;
				stepDir = 2;
			}
		}

	}

	return;
}

int VoxelHandler::GetHitVoxelId(glm::vec3 voxelWorldPos) {
	//Prevent Block from being placed inside player
	//I'm simulating that the player is 2 block tall 
	if (voxelWorldPos == m_Camera->Position || voxelWorldPos == glm::vec3(m_Camera->Position.x, m_Camera->Position.y - 1, m_Camera->Position.z)) return -1;

	//IF BLOCK POS Y ABOVE CHUNK HEIGHT DON'T CHECK FOR VOXELS
	if (voxelWorldPos.y >= CHUNK_H || voxelWorldPos.y < 0) return -1;

	int cx = floor(voxelWorldPos.x / CHUNK_W);
	int cz = floor(voxelWorldPos.z / CHUNK_D);

	ChunkCoord coord = { cx, cz };

	auto chunk = m_Chunks->find(coord);
	if (chunk != m_Chunks->end()) {

		int lx = abs(static_cast<int>(voxelWorldPos.x) % CHUNK_W);
		int ly = static_cast<int>(voxelWorldPos.y);
		int lz = abs(static_cast<int>(voxelWorldPos.z) % CHUNK_D);

		//Fix for negatives chunks
		if (coord.x < 0) lx = (CHUNK_W - 1) - lx;
		if (coord.y < 0) lz = (CHUNK_D - 1) - lz;

		unsigned int voxelId = chunk->second->GetBlock(lx, lz, ly);

		m_ChunkCoord = coord;
		m_VoxelLocalPosition = glm::ivec3(lx, ly, lz);

		return voxelId;
	}

	return -1;

}

void VoxelHandler::DestroyVoxel() {
	std::chrono::milliseconds time = Utils::GetMs();
	bool canChangeBlock = false;

	if ((time - m_LastDestroyed).count() > DELAY_DESTROY_BLOCK) canChangeBlock = true;

	if (!canChangeBlock) return;

	if (m_VoxelId > 0) {
		(*m_Chunks)[m_ChunkCoord]->SetBlock(m_VoxelLocalPosition, 0);
		(*m_Chunks)[m_ChunkCoord]->GenerateChunk();
		(*m_Chunks)[m_ChunkCoord]->SetVAO();

		Events::TriggerEvent(BlockBrokenEvent());

		UpdateAdjacentChunks();

		m_LastDestroyed = time;
	}
}

void VoxelHandler::PlaceVoxel() {
	std::chrono::milliseconds time = Utils::GetMs();
	bool canChangeBlock = false;

	if ((time - m_LastPlaced).count() > DELAY_PLACE_BLOCK) canChangeBlock = true;

	if (!canChangeBlock) return;

	if (m_VoxelId > 0) {
		int newVoxelId = GetHitVoxelId(m_VoxelWorldPos + m_VoxelNormal);
		if (newVoxelId == 0) {
			if (Inventory::s_SelectedHotbarItem->id <= 0) return;

			(*m_Chunks)[m_ChunkCoord]->SetBlock(m_VoxelLocalPosition, Inventory::s_SelectedHotbarItem->id);
			(*m_Chunks)[m_ChunkCoord]->GenerateChunk();
			(*m_Chunks)[m_ChunkCoord]->SetVAO();

			Events::TriggerEvent(BlockPlacedEvent(*(Inventory::s_SelectedHotbarItem)));

			UpdateAdjacentChunks();
		}
	}
	m_LastPlaced = time;
}

void VoxelHandler::UpdateAdjacentChunks() {
	if (m_VoxelLocalPosition.x == 0) {
		RebuildAdjacentChunk(XNEG);
	}
	else if (m_VoxelLocalPosition.x == CHUNK_W - 1) {
		RebuildAdjacentChunk(XPOS);
	}

	if (m_VoxelLocalPosition.z == 0) {
		RebuildAdjacentChunk(ZNEG);
	}
	else if (m_VoxelLocalPosition.z == CHUNK_D - 1) {
		RebuildAdjacentChunk(ZPOS);
	}
}

void VoxelHandler::RebuildAdjacentChunk(AdjacentChunkPos pos) {
	(*m_Chunks)[Chunk::GetAdjacentChunkCoord(m_ChunkCoord, pos)]->GenerateChunk();
	(*m_Chunks)[Chunk::GetAdjacentChunkCoord(m_ChunkCoord, pos)]->SetVAO();
}


void VoxelHandler::Input() {

	if (Input::isMouseButtonPressed(Mouse::ButtonLeft)) {
		DestroyVoxel();
	}
	if (Input::isMouseButtonPressed(Mouse::ButtonRight)) {
		PlaceVoxel();
	}
	if (Input::isMouseButtonPressed(Mouse::ButtonMiddle)) {
		//Should Pickup the block is looking
	}
}

void VoxelHandler::RayCast() {
	RayCasting();
	Input();
}
