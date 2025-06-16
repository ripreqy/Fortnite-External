#pragma once
#include <cstdint>

namespace offsets {
	inline constexpr uintptr_t
		Uworld = 0x181A0438,
		OwningGameInstance = 0x250,
		Levels = 0x1A8,
		Actors = 0x1A0,

		// Player Related
		LocalPlayers = 0x38,
		PlayerController = 0x30,
		PlayerState = 0x2C8,
		Acknowledgedpawn = 0x350,
		PlayerCameraManager = 0x360,
		GameState = 0x1D8,
		PlayerArray = 0x2C0,
		PawnPrivate = 0x320,

		// Weapon Related
		CurrentWeapon = 0xAD0,       // Updated from 0xA80 (wichtig für Waffenlogik)
		WeaponData = 0x560,          // Updated from 0x6B8 (wichtig für Waffenattribute)

		// Movement/Physics
		Speed = 0x1E64,              // Beibehalten (kann auch durch WeaponProjectileSpeed ersetzt werden)
		Gravity = 0x1E68,            // Beibehalten (kann auch durch WeaponProjectileGravity ersetzt werden)

		// Character Status
		BIsDying = 0x728,
		BIsDbno = 0x180,             // Updated from 0x962 (wichtig für DBNO-Status)

		// Visual/Position
		Mesh = 0x328,
		Targetedfortpawn = 0x1980,
		ComponentToWorld = 0x1E0,
		BoneArray = 0x5E8,
		RootComponent = 0x1B0,
		RelativeLocation = 0x138,

		// Inventory/Items
		ItemDefinition = 0x10,
		Rarity = 0xA2,
		ItemName = 0x40,
		bAlreadySearched = 0xE02,
		PrimaryPickupItemEntry = 0x370,

		// Team/Identification
		TeamId = 0x1291;
} 


enum class EFortRarity : uint8_t {
	EFortRarity__Common = 0,
	EFortRarity__Uncommon = 1,
	EFortRarity__Rare = 2,
	EFortRarity__Epic = 3,
	EFortRarity__Legendary = 4,
	EFortRarity__Mythic = 5,
	EFortRarity__Transcendent = 6,
	EFortRarity__Unattainable = 7,
	EFortRarity__NumRarityValues = 8,
	EFortRarity__EFortRarity_MAX = 9
};

enum EBoneIndex : int
{
	Head = 110,
	Neck = 67,
	Chest = 66,
	Pelvis = 2,
	LShoulder = 9,
	LElbow = 10,
	LHand = 11,
	RShoulder = 38,
	RElbow = 39,
	RHand = 40,
	LHip = 71,
	LKnee = 72,
	LFoot = 75,
	RHip = 78,
	RKnee = 79,
	RFoot = 82,
	BASE = 0
}; //

class FName
{
public:
	int32_t ComparisonIndex;

	std::string ToString()
	{
		return ToString(ComparisonIndex);
	}

	static std::string ToString(int32_t index)
	{
		int32_t DecryptedIndex = DecryptIndex(index);
		uint64_t NamePoolChunk = read<uint64_t>(Nunflaggedbase + 0x183C1400 + 8 * (HIWORD(DecryptedIndex) + 2)) + 2 * (uint16_t)DecryptedIndex;
		uint16_t Pool = read<uint16_t>(NamePoolChunk);

		if (((Pool ^ 0x66) & 0x3FF) == 0)
		{
			DecryptedIndex = DecryptIndex(read<int32_t>(NamePoolChunk + 2));
			NamePoolChunk = read<uint64_t>(Nunflaggedbase + 0x183C1400 + 8 * (HIWORD(DecryptedIndex) + 2)) + 2 * (uint16_t)DecryptedIndex;
			Pool = read<uint16_t>(NamePoolChunk);
		}

		int32_t Length = ((Pool ^ 0x66) & 0x3FF) * (((Pool & 0x8000u) != 0) ? 2 : 1);

		std::string NameBuffer(Length, '\0');
		Driver::ReadPhysical((PVOID)(NamePoolChunk + 2), &NameBuffer[0], Length);
		DecryptFName(&NameBuffer[0], Length);
		return NameBuffer;
	}

	static int32_t DecryptIndex(int32_t index)
	{
		if (index)
		{
			int32_t DecryptedIndex = _rotr(index - 1, 1) + 0x42DCAB0B;
			return DecryptedIndex ? DecryptedIndex : 0x42DCAB0A;
		}

		return 0;
	}

	static void DecryptFName(char* buffer, int32_t length)
	{
		if (length)
		{
			std::vector<uint8_t> EncryptedBuffer(buffer, buffer + length);

			int32_t v8 = 8618 * length + 21669824;
			for (int32_t i = 0; i < length; i++)
			{
				v8 = 8618 * v8 + 21669824;
				buffer[length - 1 - i] = char(v8 + _rotr8(EncryptedBuffer[i], 1) + 58);
			}
		}

		buffer[length] = '\0';
	}
};