#pragma once
#include <src/cache/index.h>
#include <algorithm>
#include <d3d9types.h>
#include <numbers>
#include <thirdparty/memory/memory.h>
#include "offsets.h"
#include <thirdparty/imgui/imgui.h>
#include <unordered_map>
namespace rankesp {

	inline ID3D11ShaderResourceView* Unranked = nullptr;
	inline ID3D11ShaderResourceView* Bronze1 = nullptr;
	inline ID3D11ShaderResourceView* Bronze2 = nullptr;
	inline ID3D11ShaderResourceView* Bronze3 = nullptr;
	inline ID3D11ShaderResourceView* Silver1 = nullptr;
	inline ID3D11ShaderResourceView* Silver2 = nullptr;
	inline ID3D11ShaderResourceView* Silver3 = nullptr;
	inline ID3D11ShaderResourceView* Gold1 = nullptr;
	inline ID3D11ShaderResourceView* Gold2 = nullptr;
	inline ID3D11ShaderResourceView* Gold3 = nullptr;
	inline ID3D11ShaderResourceView* Plat1 = nullptr;
	inline ID3D11ShaderResourceView* Plat2 = nullptr;
	inline ID3D11ShaderResourceView* Plat3 = nullptr;
	inline ID3D11ShaderResourceView* Diamond1 = nullptr;
	inline ID3D11ShaderResourceView* Diamond2 = nullptr;
	inline ID3D11ShaderResourceView* Diamond3 = nullptr;
	inline ID3D11ShaderResourceView* Elite = nullptr;
	inline ID3D11ShaderResourceView* Champion = nullptr;
	inline ID3D11ShaderResourceView* Unreal = nullptr;
	inline ID3D11ShaderResourceView* cat = nullptr;

}

namespace numerics {

	inline int screen_width = GetSystemMetrics( 0 );
	inline int screen_height = GetSystemMetrics( 1 );
	inline int screen_width_center = screen_width / 2;
	inline int screen_height_center = screen_height / 2;
	inline float aspect_ratio = static_cast< float >( screen_width ) / screen_height;

	inline float powf_(float _X, float _Y) {
		return std::pow(_X, _Y);  
	}

	inline double get_cross_distance(double x1, double y1, double x2, double y2) {
		return sqrt(powf_((x2 - x1), 2) + powf_((y2 - y1), 2));
	}


	class fvector {
	public:
		double x = 0.0 , y = 0.0 , z = 0.0;

		fvector( ) = default;
		fvector( double _x , double _y , double _z ) : x( _x ) , y( _y ) , z( _z ) { }

		fvector operator+( const fvector& v ) const { return { x + v.x, y + v.y, z + v.z }; }
		fvector operator-( const fvector& v ) const { return { x - v.x, y - v.y, z - v.z }; }
		fvector operator*( double scalar ) const { return { x * scalar, y * scalar, z * scalar }; }
		fvector operator/( double scalar ) const { return { x / scalar, y / scalar, z / scalar }; }

		fvector& operator*=( double scalar ) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		fvector& operator+=( const fvector& v ) { x += v.x; y += v.y; z += v.z; return *this; }
		fvector& operator-=( const fvector& v ) { x -= v.x; y -= v.y; z -= v.z; return *this; }

		bool operator==( const fvector& v ) const { return x == v.x && y == v.y && z == v.z; }
		bool operator!=( const fvector& v ) const { return !( *this == v ); }

		double dot( const fvector& v ) const { return x * v.x + y * v.y + z * v.z; }
		double distance( const fvector& v ) const { return ( *this - v ).length( ); }
		double length( ) const { return std::sqrt( x * x + y * y + z * z ); }

		void add_scale( const fvector& v , float scale )
		{
			x += v.x * scale; y += v.y * scale; z += v.z * scale;
		}
	};

	struct FFortWeaponRampingData
	{
	public:
		bool bIsRampingWeapon; // 0x0 (Size: 0x1)
		float MaxRampStacks; // 0x8 (Size: 0x28)
		float RampFireRateToAdd; // 0x30 (Size: 0x28)
		float RampGraceDuration; // 0x58 (Size: 0x28)
	};

	class fvector2d {
	public:
		inline bool ValidLocation() {
			if (x > screen_width or y > screen_height)
				return false;
			else if (x <= 0 or y <= 0)
				return false;
			else if (std::isnan(x) or std::isnan(y))
				return false;
			else
				return true;
		}
		double x = 0.0 , y = 0.0;

		fvector2d( ) = default;
		fvector2d( double _x , double _y ) : x( _x ) , y( _y ) { }

		fvector2d operator+( const fvector2d& v ) const { return { x + v.x, y + v.y }; }
		fvector2d operator-( const fvector2d& v ) const { return { x - v.x, y - v.y }; }
		fvector2d operator*( double scalar ) const { return { x * scalar, y * scalar }; }
		fvector2d operator/( double scalar ) const { return { x / scalar, y / scalar }; }

		fvector2d& operator*=( double scalar ) { x *= scalar; y *= scalar; return *this; }
		fvector2d& operator+=( const fvector2d& v ) { x += v.x; y += v.y; return *this; }
		fvector2d& operator-=( const fvector2d& v ) { x -= v.x; y -= v.y; return *this; }

		bool operator==( const fvector2d& v ) const { return x == v.x && y == v.y; }
		bool operator!=( const fvector2d& v ) const { return !( *this == v ); }

		double dot( const fvector2d& v ) const { return x * v.x + y * v.y; }
		double distance( const fvector2d& v ) const { return std::sqrt( ( v.x - x ) * ( v.x - x ) + ( v.y - y ) * ( v.y - y ) ); }
		double length( ) const { return std::sqrt( x * x + y * y ); }

		static fvector2d from_angle( double angle , double magnitude = 1.0f )
		{
			return fvector2d( cos( angle ) * magnitude , sin( angle ) * magnitude );
		}
	};
	inline bool is_in_screen(fvector2d screen_location)
	{
		return screen_location.x > 0 && screen_location.x < screen_width &&
			screen_location.y > 0 && screen_location.y < screen_height;
	}
	class frotator {
	public:
		double pitch = 0.0 , yaw = 0.0 , roll = 0.0;

		frotator( ) = default;
		frotator( double pitch , double yaw , double roll ) : pitch( pitch ) , yaw( yaw ) , roll( roll ) { }

		frotator operator+( const frotator& r ) const { return { pitch + r.pitch, yaw + r.yaw, roll + r.roll }; }
		frotator operator-( const frotator& r ) const { return { pitch - r.pitch, yaw - r.yaw, roll - r.roll }; }
		frotator operator*( double scalar ) const { return { pitch * scalar, yaw * scalar, roll * scalar }; }
		frotator operator/( double scalar ) const { return { pitch / scalar, yaw / scalar, roll / scalar }; }

		frotator& operator*=( double scalar ) { pitch *= scalar; yaw *= scalar; roll *= scalar; return *this; }
		frotator& operator+=( const frotator& r ) { pitch += r.pitch; yaw += r.yaw; roll += r.roll; return *this; }
		frotator& operator-=( const frotator& r ) { pitch -= r.pitch; yaw -= r.yaw; roll -= r.roll; return *this; }

		bool operator==( const frotator& r ) const { return pitch == r.pitch && yaw == r.yaw && roll == r.roll; }
		bool operator!=( const frotator& r ) const { return !( *this == r ); }

		double dot( const frotator& r ) const { return pitch * r.pitch + yaw * r.yaw + roll * r.roll; }
		double distance( const frotator& r ) const { return ( *this - r ).length( ); }
		double length( ) const { return std::sqrt( pitch * pitch + yaw * yaw + roll * roll ); }

		static frotator find_rotation( const fvector& start , const fvector& target )
		{
			fvector dir = ( target - start ) / ( target - start ).length( );
			return {
				std::atan2( dir.z, std::sqrt( dir.x * dir.x + dir.y * dir.y ) ) * ( 180.0 / std::numbers::pi ),
				std::atan2( dir.y, dir.x ) * ( 180.0 / std::numbers::pi ),
				0.0
			};
		}

		static double clamp_angle( double value , double min , double max )
		{
			return std::clamp( value , min , max );
		}

		static frotator clamp_rotation( const frotator& start , const frotator& end , double scale_pitch , double scale_yaw )
		{
			frotator delta = end - start;

			delta.yaw = std::fmod( delta.yaw + 360.0 , 360.0 );
			if ( delta.yaw > 180.0 ) delta.yaw -= 360.0;

			delta.pitch = clamp_angle( delta.pitch , -74.0 , 74.0 );
			return {
				delta.pitch / scale_pitch + start.pitch,
				delta.yaw / scale_yaw + start.yaw,
				0.0
			};
		}

		static void normalize_rotation( frotator& in )
		{
			in.pitch = std::fmod( in.pitch + 180.0 , 360.0 ) - 180.0;
			in.yaw = std::fmod( in.yaw + 180.0 , 360.0 ) - 180.0;
			in.roll = std::fmod( in.roll + 180.0 , 360.0 ) - 180.0;
		}
	};

	struct fplane : fvector {
		double w = 0.0;
	};

	struct alignas( 16 ) matrix_elements {
		double m11 , m12 , m13 , m14;
		double m21 , m22 , m23 , m24;
		double m31 , m32 , m33 , m34;
		double m41 , m42 , m43 , m44;

		matrix_elements( ) : m11( 0 ) , m12( 0 ) , m13( 0 ) , m14( 0 ) ,
			m21( 0 ) , m22( 0 ) , m23( 0 ) , m24( 0 ) ,
			m31( 0 ) , m32( 0 ) , m33( 0 ) , m34( 0 ) ,
			m41( 0 ) , m42( 0 ) , m43( 0 ) , m44( 0 ) {
		}
	};

	struct alignas( 16 ) dbl_matrix {
		union {
			matrix_elements elements;
			double m [ 4 ][ 4 ];
		};

		dbl_matrix( ) : elements( ) { }

		double& operator()( size_t row , size_t col ) { return m [ row ][ col ]; }
		const double& operator()( size_t row , size_t col ) const { return m [ row ][ col ]; }
	};

	struct alignas( 16 ) fmatrix : public dbl_matrix {
		numerics::fplane x_plane;
		numerics::fplane y_plane;
		numerics::fplane z_plane;
		numerics::fplane w_plane;

		fmatrix( ) : dbl_matrix( ) , x_plane( ) , y_plane( ) , z_plane( ) , w_plane( ) { }
	};

	template< class t >
	class tarray
	{
	public:
		tarray( ) : data( ) , count( ) , max_count( ) { }
		tarray( t* data , uint32_t count , uint32_t max_count ) :data( data ) , count( count ) , max_count( max_count ) { }

		t get( uintptr_t idx )
		{
			return read< t >( reinterpret_cast< uintptr_t >( data ) + ( idx * sizeof( t ) ) );
		}

		std::vector<t> get_all( )
		{
			if ( count <= 0 || count > max_count )
			{
				return {};
			}

			try
			{
				std::vector<t> buffer( count );

				Driver::ReadPhysical(
					reinterpret_cast< PVOID >( data ) ,
					buffer.data( ) ,
					sizeof( t ) * count
				);

				return buffer;
			}
			catch ( const std::bad_alloc& )
			{
				return {};
			}
		}

		uintptr_t get_address( )
		{
			return reinterpret_cast< uintptr_t >( data );
		}

		uint32_t get_count( ) const
		{
			return count;
		};

		uint32_t get_max_count( ) const
		{
			return max_count;
		};

		bool is_valid( ) const
		{
			return data != nullptr;
		};

		t& operator[]( int i )
		{
			return this->data [ i ];
		};

		t* data;
		uint32_t count;
		uint32_t max_count;
	};

	struct ESpectatorArray
	{
		uintptr_t Data;
		uint32_t Count;
		uint32_t Max;
	};

} // namespace numerics

namespace matrix_operations {

	inline D3DMATRIX matrix_multiplication( const D3DMATRIX& matrix1 , const D3DMATRIX& matrix2 )
	{
		D3DMATRIX result{};

		result._11 = matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41;
		result._12 = matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42;
		result._13 = matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43;
		result._14 = matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44;

		result._21 = matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41;
		result._22 = matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42;
		result._23 = matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43;
		result._24 = matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44;

		result._31 = matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41;
		result._32 = matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42;
		result._33 = matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43;
		result._34 = matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44;

		result._41 = matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41;
		result._42 = matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42;
		result._43 = matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43;
		result._44 = matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44;

		return result;
	}

	inline D3DMATRIX create_rotation_matrix( const numerics::frotator& rotation )
	{
		auto rad_x = rotation.pitch * float( std::numbers::pi ) / 180.f;
		auto rad_y = rotation.yaw * float( std::numbers::pi ) / 180.f;
		auto rad_z = rotation.roll * float( std::numbers::pi ) / 180.f;

		auto sp = sinf( rad_x ) , cp = cosf( rad_x );
		auto sy = sinf( rad_y ) , cy = cosf( rad_y );
		auto sr = sinf( rad_z ) , cr = cosf( rad_z );

		D3DMATRIX matrix{};
		matrix.m [ 0 ][ 0 ] = cp * cy;
		matrix.m [ 0 ][ 1 ] = cp * sy;
		matrix.m [ 0 ][ 2 ] = sp;
		matrix.m [ 0 ][ 3 ] = 0.f;

		matrix.m [ 1 ][ 0 ] = sr * sp * cy - cr * sy;
		matrix.m [ 1 ][ 1 ] = sr * sp * sy + cr * cy;
		matrix.m [ 1 ][ 2 ] = -sr * cp;
		matrix.m [ 1 ][ 3 ] = 0.f;

		matrix.m [ 2 ][ 0 ] = -( cr * sp * cy + sr * sy );
		matrix.m [ 2 ][ 1 ] = cy * sr - cr * sp * sy;
		matrix.m [ 2 ][ 2 ] = cr * cp;
		matrix.m [ 2 ][ 3 ] = 0.f;

		matrix.m [ 3 ][ 0 ] = matrix.m [ 3 ][ 1 ] = matrix.m [ 3 ][ 2 ] = 0.0f;
		matrix.m [ 3 ][ 3 ] = 1.0f;

		return matrix;
	}

	struct ftransform
	{
		numerics::fplane rot;
		numerics::fvector translation;
		char pad_38 [ 4 ];
		numerics::fvector scale = { 1.0, 1.0, 1.0 };
		char pad_58 [ 4 ];

		inline D3DMATRIX to_matrix_with_scale( ) const
		{
			D3DMATRIX m{};
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			auto x2 = rot.x + rot.x , y2 = rot.y + rot.y , z2 = rot.z + rot.z;
			auto xx2 = rot.x * x2 , yy2 = rot.y * y2 , zz2 = rot.z * z2;
			m._11 = ( 1.0f - ( yy2 + zz2 ) ) * scale.x;
			m._22 = ( 1.0f - ( xx2 + zz2 ) ) * scale.y;
			m._33 = ( 1.0f - ( xx2 + yy2 ) ) * scale.z;

			auto yz2 = rot.y * z2 , wx2 = rot.w * x2;
			m._32 = ( yz2 - wx2 ) * scale.z;
			m._23 = ( yz2 + wx2 ) * scale.y;

			auto xy2 = rot.x * y2 , wz2 = rot.w * z2;
			m._21 = ( xy2 - wz2 ) * scale.y;
			m._12 = ( xy2 + wz2 ) * scale.x;

			auto xz2 = rot.x * z2 , wy2 = rot.w * y2;
			m._31 = ( xz2 + wy2 ) * scale.z;
			m._13 = ( xz2 - wy2 ) * scale.x;

			m._14 = m._24 = m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

} // namespace matrix_operations

namespace bones {

	inline numerics::fvector get_bone_position( uintptr_t mesh , int boneId ) {
		int32_t active_transforms = read<int32_t>( mesh + offsets::BoneArray + 0x48 ) * 0x10;
		uintptr_t bone_array_ptr = mesh + offsets::BoneArray + active_transforms;
		auto bone_array = read<numerics::tarray<matrix_operations::ftransform>>( bone_array_ptr );
		auto bone = bone_array.get( boneId );
		auto component_to_world = read<matrix_operations::ftransform>( mesh + offsets::ComponentToWorld );

		auto bone_matrix = matrix_operations::matrix_multiplication( bone.to_matrix_with_scale( ) , component_to_world.to_matrix_with_scale( ) );
		return { bone_matrix._41, bone_matrix._42, bone_matrix._43 };
	}
}

namespace udata {

	struct world
	{
		uintptr_t gworld;
		uintptr_t owning_game_instance;
		uintptr_t local_player;
		uintptr_t player_controller;
		uintptr_t player_state;
		uintptr_t local_pawn;
		uintptr_t root_component;
		uintptr_t Spectator;

		int team_id;

		uintptr_t game_state;
		uintptr_t mesh;
		uintptr_t closeest_actor;
		std::vector<uintptr_t> player_array;
	};

	struct actor
	{
		uintptr_t player_state;
		uintptr_t pawn_private;
		uintptr_t mesh;

		int team_id;
		int kill_score;

		uintptr_t character_movement;
		uintptr_t current_weapon;

		numerics::fvector location;
		bool knocked;
		bool visibility;
	};

	typedef struct environment
	{
		uintptr_t actor;
		std::string name;
		bool is_pickup;
		float distance;
	} environment;
	inline std::vector<environment> inline environment_list;
	inline std::vector<environment> temp_environment_list;

	inline world world_t;
	inline std::vector<actor>( actor_t );

} // namespace udata

namespace camera {

	inline uintptr_t view_state = 0;

	inline numerics::fvector location{};
	inline numerics::frotator rotation{};
	inline float fov = 0.0f;

	inline void setup_camera( )
	{
		uintptr_t view_matrix = read<uintptr_t>( udata::world_t.local_player + 0xd0 );

		view_state = read<uintptr_t>( view_matrix + 0x8 );
	}

	inline void update_camera( )
	{
		if ( !view_state )
		{
			setup_camera( );
		}

		auto projection = read<numerics::fmatrix>( view_state + 0x940 );

		rotation.pitch = asin( projection.z_plane.w ) * 180.0f / std::numbers::pi;
		rotation.yaw = atan2( projection.y_plane.w , projection.x_plane.w ) * 180.0f / std::numbers::pi;
		rotation.roll = 0.0f;

		location.x = projection.m [ 3 ][ 0 ];
		location.y = projection.m [ 3 ][ 1 ];
		location.z = projection.m [ 3 ][ 2 ];

		auto fov_radians = 2.0f * atanf( 1.0f / static_cast< float >( read<double>( view_state + 0x740 ) ) );
		fov = fov_radians * 180.0f / std::numbers::pi;
	}
}

namespace world_to_screen {
	inline D3DMATRIX rotation_matrix;
	inline float inv_fov;

	inline numerics::fvector2d project_to_screen( const numerics::fvector& location )
	{
		auto& axis_x = rotation_matrix.m [ 0 ];
		auto& axis_y = rotation_matrix.m [ 1 ];
		auto& axis_z = rotation_matrix.m [ 2 ];

		auto delta = location - camera::location;

		auto transformed_x = delta.dot( numerics::fvector( axis_y [ 0 ] , axis_y [ 1 ] , axis_y [ 2 ] ) );
		auto transformed_y = delta.dot( numerics::fvector( axis_z [ 0 ] , axis_z [ 1 ] , axis_z [ 2 ] ) );
		auto transformed_z = max( delta.dot( numerics::fvector( axis_x [ 0 ] , axis_x [ 1 ] , axis_x [ 2 ] ) ) , 1.0f );

		auto screen_x = numerics::screen_width_center + transformed_x * ( numerics::screen_height_center / inv_fov ) / transformed_z;
		auto screen_y = numerics::screen_height_center - transformed_y * ( numerics::screen_height_center / inv_fov ) / transformed_z;

		return numerics::fvector2d( screen_x , screen_y );
	}

	inline bool is_within_screen_bounds( const numerics::fvector2d& screen_location )
	{
		return screen_location.x >= 0 && screen_location.x <= numerics::screen_width && screen_location.y >= 0 && screen_location.y <= numerics::screen_height;
	}

} // namespace world_to_screen

struct bounds
{
	double min_x , max_x , min_y , max_y;
};









namespace player {
	inline void h_custom(float r, bool filled, bool rainbow, bool toMouse, float rainbowSpeed)
	{
		auto& io = ImGui::GetIO();

		ImVec2 center = toMouse ? ImVec2(io.MousePos.x, io.MousePos.y) : ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
		auto drawList = ImGui::GetBackgroundDrawList();
		int sides = 100;
		for (int i = 0; i < sides; ++i)
		{
			auto pos = center;
			float angle = (i / static_cast<float>(sides)) * 2 * M_PI;
			auto lastPos = ImVec2(pos.x + cos(angle) * r, pos.y + sin(angle) * r);
			auto nextPos = ImVec2(pos.x + cos(angle + 2 * M_PI / sides) * r, pos.y + sin(angle + 2 * M_PI / sides) * r);


			ImU32 currentColor = rainbow ? ImGui::ColorConvertFloat4ToU32(ImColor::HSV((fmod(ImGui::GetTime() * rainbowSpeed, 5.0f) / 5.0f - i / static_cast<float>(sides)) + 1.0f, 0.5f, 1.0f)) : IM_COL32(255, 255, 255, 255);

			ImU32 fillCol = filled ? ImGui::ColorConvertFloat4ToU32({ ImGui::ColorConvertU32ToFloat4(currentColor).x, ImGui::ColorConvertU32ToFloat4(currentColor).y, ImGui::ColorConvertU32ToFloat4(currentColor).z, 0.2f }) : 0; // 0.2f = fill opacity
			drawList->AddLine(lastPos, nextPos, currentColor, 1.5f); // main 
		}
	}


	inline bounds get_player_bounds( uintptr_t mesh )
	{
		const std::vector<int> bone_indices = { 110, 71, 78, 72, 79, 83, 76, 9, 38, 33, 62 };
		std::vector<numerics::fvector2d> bone_screen_coords( bone_indices.size( ) );

		std::transform( bone_indices.begin( ) , bone_indices.end( ) , bone_screen_coords.begin( ) , [ & ] ( int index ) {
			return world_to_screen::project_to_screen( bones::get_bone_position( mesh , index ) );
			} );

		auto [min_x , max_x] = std::minmax_element( bone_screen_coords.begin( ) , bone_screen_coords.end( ) , [ ] ( const auto& a , const auto& b ) { return a.x < b.x; } );
		auto [min_y , max_y] = std::minmax_element( bone_screen_coords.begin( ) , bone_screen_coords.end( ) , [ ] ( const auto& a , const auto& b ) { return a.y < b.y; } );

		const auto box_height = max_y->y - min_y->y;
		const auto box_width = max_x->x - min_x->x;

		const auto width_offset = box_width * 0.175f;
		const auto height_offset_top = box_height * 0.125f;
		const auto height_offset_bottom = box_height * 0.05f;

		return {
			min_x->x - width_offset,
			max_x->x + width_offset,
			min_y->y - height_offset_top,
			max_y->y + height_offset_bottom
		};
	}

	inline auto IsVisible(uintptr_t skeletal_mesh) -> bool {

		auto Seconds = read<double>(udata::world_t.gworld + 0x158);
		auto LastRenderTime = read<float>(skeletal_mesh + 0x32C);
		return Seconds - LastRenderTime <= 0.06f;

	}
	inline numerics::fvector PredictLocation(numerics::fvector target, numerics::fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
	{
		float horizontalTime = distance / projectileSpeed;
		float verticalTime = distance / projectileSpeed; target.x += targetVelocity.x * horizontalTime;
		target.y += targetVelocity.y * horizontalTime;
		target.z += targetVelocity.z * verticalTime + abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);
		return target;
	}
	inline ImVec4 GetRankColor(int32_t Tier)
	{
		if (Tier == 0 || Tier == 1 || Tier == 2)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Bronze
		else if (Tier == 3 || Tier == 4 || Tier == 5)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Silver
		else if (Tier == 6 || Tier == 7 || Tier == 8)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Gold
		else if (Tier == 9 || Tier == 10 || Tier == 11)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);// Platinum
		else if (Tier == 12 || Tier == 13 || Tier == 14)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Diamond
		else if (Tier == 15)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Elite
		else if (Tier == 16)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);// Champion
		else if (Tier == 17)
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Unreal
		else
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Unranked
	}
	inline ImTextureID GetRank(ImDrawList* drawlist, int32_t Tier, ImVec2 text1, ImVec2 text22, ImColor rankedColor) {
		ID3D11ShaderResourceView* texture = nullptr;
		switch (Tier) {
		case 0:  texture = rankesp::Bronze1; break;
		case 1:  texture = rankesp::Bronze2; break;
		case 2:  texture = rankesp::Bronze3; break;
		case 3:  texture = rankesp::Silver1; break;
		case 4:  texture = rankesp::Silver2; break;
		case 5:  texture = rankesp::Silver3; break;
		case 6:  texture = rankesp::Gold1; break;
		case 7:  texture = rankesp::Gold2; break;
		case 8:  texture = rankesp::Gold3; break;
		case 9:  texture = rankesp::Plat1; break;
		case 10: texture = rankesp::Plat2; break;
		case 11: texture = rankesp::Plat3; break;
		case 12: texture = rankesp::Diamond1; break;
		case 13: texture = rankesp::Diamond2; break;
		case 14: texture = rankesp::Diamond3; break;
		case 15: texture = rankesp::Elite; break;
		case 16: texture = rankesp::Champion; break;
		case 17: texture = rankesp::Unreal; break;
		default: texture = rankesp::Unranked; break;
		}

		if (texture) {
			drawlist->AddImage(reinterpret_cast<ImTextureID>(texture), text1, text22, ImVec2(0, 0), ImVec2(1, 1), rankedColor);
		}

		return reinterpret_cast<ImTextureID>(texture);
	}
	inline ImColor Plat(const std::string& platform) {
		if (platform == ("G")) return ImColor(128, 224, 0, 200);
		if (platform == ("H") || platform == ("H")) return ImColor(0, 96, 192, 255);
		if (platform == ("A")) return ImColor(255, 255, 255);
		if (platform == ("F")) return ImColor(255, 0, 0);
		if (platform == ("E")) return ImColor(255, 0, 0);
		if (platform == ("C")) return ImColor(255, 0, 0);
		if (platform == ("B")) return ImColor(255, 0, 0);
		if (platform == ("D")) return ImColor(255, 0, 0);

		return ImColor(255, 255, 255);
	}
	inline __forceinline auto platform(uintptr_t PlayerState) -> std::string
	{

		uintptr_t test_platform = read<uintptr_t>(PlayerState + 0x430); //platform offsets
		if (!test_platform) return "Unknown";
		wchar_t platform[64]{ 0 };
		Driver::ReadPhysical(reinterpret_cast<PVOID>(test_platform), reinterpret_cast<uint8_t*>(platform), sizeof(platform));
		std::wstring platform_wstring(platform);
		static const std::unordered_map<std::wstring, std::string> platform_map = {
			{L"XBL", "G"}, {L"XSX", "G"}, {L"PSN", "H"}, {L"PS5", "H"},
			{L"WIN", "A"}, {L"SWT", "F"}, {L"AND", "E"}, {L"MAC", "C"},
			{L"LNX", "B"}, {L"IOS", "D"}
		};

		for (const auto& pair : platform_map) {
			if (platform_wstring.find(pair.first) != std::wstring::npos) {
				return pair.second;
			}
		}
		return "Unknown";
	}
	inline auto GetPlayerName(uintptr_t player_state) -> std::string
	{

		__int64 FString = read<__int64>(player_state + 0xb08);
		int Length = read<int>(FString + 16);
		uintptr_t FText = read<__int64>(FString + 8);

		__int64 v6 = Length;
		if (!v6) return std::string("Bot");

		wchar_t* NameBuffer = new wchar_t[Length];
		Driver::ReadPhysical((PVOID)FText, NameBuffer, Length * sizeof(wchar_t));

		char v21;
		int v22;
		int i;
		int v25;
		WORD* v23;

		v21 = v6 - 1;
		if (!(DWORD)v6)
			v21 = 0;
		v22 = 0;
		v23 = (WORD*)NameBuffer;
		for (i = (v21) & 3; ; *v23++ += i & 7)
		{
			v25 = v6 - 1;
			if (!(DWORD)v6)
				v25 = 0;
			if (v22 >= v25)
				break;
			i += 3;
			++v22;
		}

		std::wstring wbuffer{ NameBuffer };

		return std::string(wbuffer.begin(), wbuffer.end());

	}
	inline std::vector<ImVec2> GetCatmullRomSpline(const std::vector<ImVec2>& controlPoints, int segments)
	{
		std::vector<ImVec2> spline;
		if (controlPoints.size() < 2)
			return spline;

		// Duplicate the endpoints to better handle endpoints.
		std::vector<ImVec2> pts;
		pts.push_back(controlPoints.front());
		pts.insert(pts.end(), controlPoints.begin(), controlPoints.end());
		pts.push_back(controlPoints.back());

		// For each segment between pts[i] and pts[i+1], interpolate using pts[i-1], pts[i], pts[i+1], pts[i+2]
		for (size_t i = 1; i < pts.size() - 2; i++)
		{
			for (int j = 0; j < segments; j++)
			{
				float t = j / (float)segments;
				float t2 = t * t;
				float t3 = t2 * t;
				ImVec2 p0 = pts[i - 1];
				ImVec2 p1 = pts[i];
				ImVec2 p2 = pts[i + 1];
				ImVec2 p3 = pts[i + 2];

				ImVec2 point;
				point.x = 0.5f * ((2 * p1.x) + (-p0.x + p2.x) * t + (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 + (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3);
				point.y = 0.5f * ((2 * p1.y) + (-p0.y + p2.y) * t + (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 + (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3);
				spline.push_back(point);
			}
		}
		// Ensure the last control point is added.
		spline.push_back(pts[pts.size() - 2]);
		return spline;
	}
	inline void DrawSkeleton(ImDrawList* drawlist, bool curved, bool spaghetti, const ImColor& col, int tickness, bool outline, uintptr_t mesh)
	{
		ImColor lineColor = col; // Cyan (Full Alpha)
		float outlineThickness = tickness;
		float thickness = tickness;

		if (curved && !spaghetti)
		{
			// Compute shoulder center
			numerics::fvector leftShoulder3D = bones::get_bone_position(mesh, EBoneIndex::LShoulder);
			numerics::fvector rightShoulder3D = bones::get_bone_position(mesh, EBoneIndex::RShoulder);
			numerics::fvector2d leftShoulderScreen = world_to_screen::project_to_screen(leftShoulder3D);
			numerics::fvector2d rightShoulderScreen = world_to_screen::project_to_screen(rightShoulder3D);
			ImVec2 centerShoulders((leftShoulderScreen.x + rightShoulderScreen.x) / 2.0f,
				(leftShoulderScreen.y + rightShoulderScreen.y) / 2.0f);

			// Build continuous bone chains with arms using the computed center:
			// Left arm: center -> LShoulder -> LElbow -> LHand
			std::vector<ImVec2> leftArmChain;
			leftArmChain.push_back(centerShoulders);
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LShoulder);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftArmChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LElbow);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftArmChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LHand);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftArmChain.push_back(ImVec2(scr.x, scr.y));
			}

			// Right arm: center -> RShoulder -> RElbow -> RHand
			std::vector<ImVec2> rightArmChain;
			rightArmChain.push_back(centerShoulders);
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RShoulder);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightArmChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RElbow);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightArmChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RHand);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightArmChain.push_back(ImVec2(scr.x, scr.y));
			}

			// Left leg: Pelvis -> LHip -> LKnee -> LFoot
			std::vector<ImVec2> leftLegChain;
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::Pelvis);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LHip);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LKnee);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::LFoot);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				leftLegChain.push_back(ImVec2(scr.x, scr.y));
			}

			// Right leg: Pelvis -> RHip -> RKnee -> RFoot
			std::vector<ImVec2> rightLegChain;
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::Pelvis);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RHip);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RKnee);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightLegChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::RFoot);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				rightLegChain.push_back(ImVec2(scr.x, scr.y));
			}

			// Spine: Neck -> Pelvis
			std::vector<ImVec2> spineChain;
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::Neck);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				spineChain.push_back(ImVec2(scr.x, scr.y));
			}
			{
				numerics::fvector pos = bones::get_bone_position(mesh, EBoneIndex::Pelvis);
				numerics::fvector2d scr = world_to_screen::project_to_screen(pos);
				spineChain.push_back(ImVec2(scr.x, scr.y));
			}

			// Collect all chains to draw.
			std::vector<std::vector<ImVec2>> chainsToDraw = { leftArmChain, rightArmChain, leftLegChain, rightLegChain, spineChain };

			// For each chain, generate and draw a Catmull-Rom spline.
			for (const auto& controlPoints : chainsToDraw)
			{
				if (controlPoints.size() < 2)
					continue;
				std::vector<ImVec2> splinePoints = GetCatmullRomSpline(controlPoints, 10);
				if (splinePoints.size() < 2)
					continue;
				if (outline)
					drawlist->AddPolyline(splinePoints.data(), splinePoints.size(), lineColor, false, outlineThickness);
				else
					drawlist->AddPolyline(splinePoints.data(), splinePoints.size(), lineColor, false, thickness);
			}
		}
		else if (spaghetti)
		{
				// Spaghetti effect: draw individual bone pairs with a wiggly offset.
				std::vector<std::pair<EBoneIndex, EBoneIndex>> skeletonPairs = {
					{ EBoneIndex::Neck, EBoneIndex::Pelvis },
					{ EBoneIndex::LShoulder, EBoneIndex::LElbow },
					{ EBoneIndex::LElbow, EBoneIndex::LHand },
					{ EBoneIndex::RShoulder, EBoneIndex::RElbow },
					{ EBoneIndex::RElbow, EBoneIndex::RHand },
					{ EBoneIndex::Pelvis, EBoneIndex::LHip },
					{ EBoneIndex::Pelvis, EBoneIndex::RHip },
					{ EBoneIndex::LHip, EBoneIndex::LKnee },
					{ EBoneIndex::LKnee, EBoneIndex::LFoot },
					{ EBoneIndex::RHip, EBoneIndex::RKnee },
					{ EBoneIndex::RKnee, EBoneIndex::RFoot },
					{ EBoneIndex::LShoulder, EBoneIndex::RShoulder }
				};

				for (const auto& bonePair : skeletonPairs)
				{
					numerics::fvector boneStart = bones::get_bone_position(mesh, bonePair.first);
					numerics::fvector boneEnd = bones::get_bone_position(mesh, bonePair.second);
					numerics::fvector2d startScreenPosition = world_to_screen::project_to_screen(boneStart);
					numerics::fvector2d endScreenPosition = world_to_screen::project_to_screen(boneEnd);
					if (!startScreenPosition.ValidLocation() || !endScreenPosition.ValidLocation())
						continue;

					const int numSegments = 20;
					std::vector<ImVec2> points;
					points.reserve(numSegments + 1);
					numerics::fvector2d diff = endScreenPosition - startScreenPosition;
					float totalLength = sqrtf(diff.x * diff.x + diff.y * diff.y);
					numerics::fvector2d dir = { diff.x / totalLength, diff.y / totalLength };
					numerics::fvector2d perp = { -dir.y, dir.x };

					float amplitude = 5.0f;
					float frequency = 3.0f;
					for (int i = 0; i <= numSegments; i++)
					{
						float t = i / (float)numSegments;
						numerics::fvector2d base = startScreenPosition + diff * t;
						float offset = amplitude * sinf(t * frequency * 2.0f * 3.14159f);
						numerics::fvector2d point = base + perp * offset;
						points.push_back(ImVec2(point.x, point.y));
					}
					if (outline)
						drawlist->AddPolyline(points.data(), points.size(), lineColor, false, outlineThickness);
					drawlist->AddPolyline(points.data(), points.size(), lineColor, false, thickness);
				}
		}
		else
		{
			std::vector<std::pair<EBoneIndex, EBoneIndex>> skeletonPairs = {
				{ EBoneIndex::Neck, EBoneIndex::Pelvis },
				{ EBoneIndex::LShoulder, EBoneIndex::LElbow },
				{ EBoneIndex::LElbow, EBoneIndex::LHand },
				{ EBoneIndex::RShoulder, EBoneIndex::RElbow },
				{ EBoneIndex::RElbow, EBoneIndex::RHand },
				{ EBoneIndex::Pelvis, EBoneIndex::LHip },
				{ EBoneIndex::Pelvis, EBoneIndex::RHip },
				{ EBoneIndex::LHip, EBoneIndex::LKnee },
				{ EBoneIndex::LKnee, EBoneIndex::LFoot },
				{ EBoneIndex::RHip, EBoneIndex::RKnee },
				{ EBoneIndex::RKnee, EBoneIndex::RFoot },
				{ EBoneIndex::LShoulder, EBoneIndex::RShoulder }
			};

			for (const auto& bonePair : skeletonPairs)
			{
				numerics::fvector boneStart = bones::get_bone_position(mesh, bonePair.first);
				numerics::fvector boneEnd = bones::get_bone_position(mesh, bonePair.second);
				numerics::fvector2d startScreenPosition = world_to_screen::project_to_screen(boneStart);
				numerics::fvector2d endScreenPosition = world_to_screen::project_to_screen(boneEnd);
				if (!startScreenPosition.ValidLocation() || !endScreenPosition.ValidLocation())
					continue;


				if (outline)
					drawlist->AddLine(ImVec2(startScreenPosition.x, startScreenPosition.y),
						ImVec2(endScreenPosition.x, endScreenPosition.y),
						lineColor,
						outlineThickness);

				drawlist->AddLine(ImVec2(startScreenPosition.x, startScreenPosition.y),
					ImVec2(endScreenPosition.x, endScreenPosition.y),
					lineColor,
					thickness);
			}
		}
	}

	inline void DrawPlayerArrow(numerics::fvector2d pos, bool filled, int fov)
	{
		const auto ARROW_SIZE = 13.0f;

		auto Center = ImVec2(numerics::screen_width / 2, numerics::screen_height / 2);
		auto Direction = ImVec2(pos.x - Center.x, pos.y - Center.y);

		auto Length = sqrtf(Direction.x * Direction.x + Direction.y * Direction.y);
		Direction.x /= Length;
		Direction.y /= Length;

		auto Angle = atan2f(Direction.y, Direction.x);
		auto ArrowPos = ImVec2(Center.x + Direction.x * fov, Center.y + Direction.y * fov);

		// Obtener el tiempo actual para calcular los colores RGB
		float time = ImGui::GetTime();
		int r = static_cast<int>(sin(time * 2.0f) * 127.5f + 127.5f);
		int g = static_cast<int>(sin(time * 2.0f + 2.0f) * 127.5f + 127.5f);
		int b = static_cast<int>(sin(time * 2.0f + 4.0f) * 127.5f + 127.5f);
		ImColor rgbColor(r, g, b);

		if (!filled) {
			ImGui::GetBackgroundDrawList()->AddTriangle(
				ImVec2(ArrowPos.x + cosf(Angle) * ARROW_SIZE, ArrowPos.y + sinf(Angle) * ARROW_SIZE),
				ImVec2(ArrowPos.x + cosf(Angle + 2.35619f) * ARROW_SIZE, ArrowPos.y + sinf(Angle + 2.35619f) * ARROW_SIZE),
				ImVec2(ArrowPos.x + cosf(Angle - 2.35619f) * ARROW_SIZE, ArrowPos.y + sinf(Angle - 2.35619f) * ARROW_SIZE), rgbColor);
		}
		else {
			ImGui::GetBackgroundDrawList()->AddTriangleFilled(
				ImVec2(ArrowPos.x + cosf(Angle) * ARROW_SIZE, ArrowPos.y + sinf(Angle) * ARROW_SIZE),
				ImVec2(ArrowPos.x + cosf(Angle + 2.35619f) * ARROW_SIZE, ArrowPos.y + sinf(Angle + 2.35619f) * ARROW_SIZE),
				ImVec2(ArrowPos.x + cosf(Angle - 2.35619f) * ARROW_SIZE, ArrowPos.y + sinf(Angle - 2.35619f) * ARROW_SIZE), rgbColor);
		}

		// does distance
		// if (drawDistance) {
		//     ImGui::GetOverlayDrawList()->AddText(ArrowPos - ImGui::CalcTextSize(std::to_string(distance).c_str()) / 2, IM_COL32_WHITE, std::to_string(distance).c_str());
		// }
	}
}