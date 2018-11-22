// Includes
//=========

#include "cMatrix_transformation.h"

#include "cQuaternion.h"
#include "sVector.h"

#include <cmath>

// Interface
//==========

// Multiplication
//---------------

eae6320::Math::sVector eae6320::Math::cMatrix_transformation::operator *( const sVector i_rhs ) const
{
	return sVector(
		( m_00 * i_rhs.x ) + ( m_01 * i_rhs.y ) + ( m_02 * i_rhs.z ) + m_03,
		( m_10 * i_rhs.x ) + ( m_11 * i_rhs.y ) + ( m_12 * i_rhs.z ) + m_13,
		( m_20 * i_rhs.x ) + ( m_21 * i_rhs.y ) + ( m_22 * i_rhs.z ) + m_23
	);
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::operator *( const cMatrix_transformation& i_rhs ) const
{
	return cMatrix_transformation(
		( m_00 * i_rhs.m_00 ) + ( m_01 * i_rhs.m_10 ) + ( m_02 * i_rhs.m_20 ) + ( m_03 * i_rhs.m_30 ),
		( m_10 * i_rhs.m_00 ) + ( m_11 * i_rhs.m_10 ) + ( m_12 * i_rhs.m_20 ) + ( m_13 * i_rhs.m_30 ),
		( m_20 * i_rhs.m_00 ) + ( m_21 * i_rhs.m_10 ) + ( m_22 * i_rhs.m_20 ) + ( m_23 * i_rhs.m_30 ),
		( m_30 * i_rhs.m_00 ) + ( m_31 * i_rhs.m_10 ) + ( m_32 * i_rhs.m_20 ) + ( m_33 * i_rhs.m_30 ),

		( m_00 * i_rhs.m_01 ) + ( m_01 * i_rhs.m_11 ) + ( m_02 * i_rhs.m_21 ) + ( m_03 * i_rhs.m_31 ),
		( m_10 * i_rhs.m_01 ) + ( m_11 * i_rhs.m_11 ) + ( m_12 * i_rhs.m_21 ) + ( m_13 * i_rhs.m_31 ),
		( m_20 * i_rhs.m_01 ) + ( m_21 * i_rhs.m_11 ) + ( m_22 * i_rhs.m_21 ) + ( m_23 * i_rhs.m_31 ),
		( m_30 * i_rhs.m_01 ) + ( m_31 * i_rhs.m_11 ) + ( m_32 * i_rhs.m_21 ) + ( m_33 * i_rhs.m_31 ),

		( m_00 * i_rhs.m_02 ) + ( m_01 * i_rhs.m_12 ) + ( m_02 * i_rhs.m_22 ) + ( m_03 * i_rhs.m_32 ),
		( m_10 * i_rhs.m_02 ) + ( m_11 * i_rhs.m_12 ) + ( m_12 * i_rhs.m_22 ) + ( m_13 * i_rhs.m_32 ),
		( m_20 * i_rhs.m_02 ) + ( m_21 * i_rhs.m_12 ) + ( m_22 * i_rhs.m_22 ) + ( m_23 * i_rhs.m_32 ),
		( m_30 * i_rhs.m_02 ) + ( m_31 * i_rhs.m_12 ) + ( m_32 * i_rhs.m_22 ) + ( m_33 * i_rhs.m_32 ),

		( m_00 * i_rhs.m_03 ) + ( m_01 * i_rhs.m_13 ) + ( m_02 * i_rhs.m_23 ) + ( m_03 * i_rhs.m_33 ),
		( m_10 * i_rhs.m_03 ) + ( m_11 * i_rhs.m_13 ) + ( m_12 * i_rhs.m_23 ) + ( m_13 * i_rhs.m_33 ),
		( m_20 * i_rhs.m_03 ) + ( m_21 * i_rhs.m_13 ) + ( m_22 * i_rhs.m_23 ) + ( m_23 * i_rhs.m_33 ),
		( m_30 * i_rhs.m_03 ) + ( m_31 * i_rhs.m_13 ) + ( m_32 * i_rhs.m_23 ) + ( m_33 * i_rhs.m_33 )
	);
}

const eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::ConcatenateAffine(
	const cMatrix_transformation& i_nextTransform, const cMatrix_transformation& i_firstTransform )
{
	// A few simplifying assumptions can be made for affine transformations vs. general 4x4 matrix multiplication
	return cMatrix_transformation(
		( i_nextTransform.m_00 * i_firstTransform.m_00 ) + ( i_nextTransform.m_01 * i_firstTransform.m_10 ) + ( i_nextTransform.m_02 * i_firstTransform.m_20 ),
		( i_nextTransform.m_10 * i_firstTransform.m_00 ) + ( i_nextTransform.m_11 * i_firstTransform.m_10 ) + ( i_nextTransform.m_12 * i_firstTransform.m_20 ),
		( i_nextTransform.m_20 * i_firstTransform.m_00 ) + ( i_nextTransform.m_21 * i_firstTransform.m_10 ) + ( i_nextTransform.m_22 * i_firstTransform.m_20 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_01 ) + ( i_nextTransform.m_01 * i_firstTransform.m_11 ) + ( i_nextTransform.m_02 * i_firstTransform.m_21 ),
		( i_nextTransform.m_10 * i_firstTransform.m_01 ) + ( i_nextTransform.m_11 * i_firstTransform.m_11 ) + ( i_nextTransform.m_12 * i_firstTransform.m_21 ),
		( i_nextTransform.m_20 * i_firstTransform.m_01 ) + ( i_nextTransform.m_21 * i_firstTransform.m_11 ) + ( i_nextTransform.m_22 * i_firstTransform.m_21 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_02 ) + ( i_nextTransform.m_01 * i_firstTransform.m_12 ) + ( i_nextTransform.m_02 * i_firstTransform.m_22 ),
		( i_nextTransform.m_10 * i_firstTransform.m_02 ) + ( i_nextTransform.m_11 * i_firstTransform.m_12 ) + ( i_nextTransform.m_12 * i_firstTransform.m_22 ),
		( i_nextTransform.m_20 * i_firstTransform.m_02 ) + ( i_nextTransform.m_21 * i_firstTransform.m_12 ) + ( i_nextTransform.m_22 * i_firstTransform.m_22 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_03 ) + ( i_nextTransform.m_01 * i_firstTransform.m_13 ) + ( i_nextTransform.m_02 * i_firstTransform.m_23 ) + i_nextTransform.m_03,
		( i_nextTransform.m_10 * i_firstTransform.m_03 ) + ( i_nextTransform.m_11 * i_firstTransform.m_13 ) + ( i_nextTransform.m_12 * i_firstTransform.m_23 ) + i_nextTransform.m_13,
		( i_nextTransform.m_20 * i_firstTransform.m_03 ) + ( i_nextTransform.m_21 * i_firstTransform.m_13 ) + ( i_nextTransform.m_22 * i_firstTransform.m_23 ) + i_nextTransform.m_23,
		1.0f
	);
}

// Access
//-------

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetRightDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_00 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetUpDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_01 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetBackDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_02 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetTranslation() const
{
	return *reinterpret_cast<const sVector*>( &m_03 );
}

// Camera
//-------

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(
	const cQuaternion& i_cameraOrientation, const sVector i_cameraPosition )
{
	return CreateWorldToCameraTransform( cMatrix_transformation( i_cameraOrientation, i_cameraPosition ) );
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform( const cMatrix_transformation& i_transform_localCameraToWorld )
{
	// Many simplifying assumptions can be made in order to create the inverse
	// because in our class a camera can only ever have rotation and translation
	// (i.e. it can't be scaled)
	return cMatrix_transformation(
		i_transform_localCameraToWorld.m_00, i_transform_localCameraToWorld.m_01, i_transform_localCameraToWorld.m_02, 0.0f,
		i_transform_localCameraToWorld.m_10, i_transform_localCameraToWorld.m_11, i_transform_localCameraToWorld.m_12, 0.0f,
		i_transform_localCameraToWorld.m_20, i_transform_localCameraToWorld.m_21, i_transform_localCameraToWorld.m_22, 0.0f,

		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_00 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_10 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_20 ),
		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_01 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_11 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_21 ),
		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_02 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_12 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_22 ),

		1.0f );
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
	const float i_verticalFieldOfView_inRadians,
	const float i_aspectRatio,
	const float i_z_nearPlane, const float i_z_farPlane )
{
	const auto yScale = 1.0f / std::tan( i_verticalFieldOfView_inRadians * 0.5f );
	const auto xScale = yScale / i_aspectRatio;
#if defined( EAE6320_PLATFORM_D3D )
	const auto zDistanceScale = i_z_farPlane / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zDistanceScale, -1.0f,
		0.0f, 0.0f, i_z_nearPlane * zDistanceScale, 0.0f );
#elif defined( EAE6320_PLATFORM_GL )
	const auto zDistanceScale = 1.0f / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, ( i_z_nearPlane + i_z_farPlane ) * zDistanceScale, -1.0f,
		0.0f, 0.0f, ( 2.0f * i_z_nearPlane * i_z_farPlane ) * zDistanceScale, 0.0f );
#endif
}

// Initialization / Shut Down
//---------------------------

eae6320::Math::cMatrix_transformation::cMatrix_transformation( const cQuaternion& i_rotation, const sVector i_translation )
	:
	m_30( 0.0f ), m_31( 0.0f ), m_32( 0.0f ),
	m_03( i_translation.x ), m_13( i_translation.y ), m_23( i_translation.z ),
	m_33( 1.0f )
{
	const auto _2x = i_rotation.m_x + i_rotation.m_x;
	const auto _2y = i_rotation.m_y + i_rotation.m_y;
	const auto _2z = i_rotation.m_z + i_rotation.m_z;
	const auto _2xx = i_rotation.m_x * _2x;
	const auto _2xy = _2x * i_rotation.m_y;
	const auto _2xz = _2x * i_rotation.m_z;
	const auto _2xw = _2x * i_rotation.m_w;
	const auto _2yy = _2y * i_rotation.m_y;
	const auto _2yz = _2y * i_rotation.m_z;
	const auto _2yw = _2y * i_rotation.m_w;
	const auto _2zz = _2z * i_rotation.m_z;
	const auto _2zw = _2z * i_rotation.m_w;

	m_00 = 1.0f - _2yy - _2zz;
	m_01 = _2xy - _2zw;
	m_02 = _2xz + _2yw;

	m_10 = _2xy + _2zw;
	m_11 = 1.0f - _2xx - _2zz;
	m_12 = _2yz - _2xw;

	m_20 = _2xz - _2yw;
	m_21 = _2yz + _2xw;
	m_22 = 1.0f - _2xx - _2yy;
}

// Implementation
//===============

// Initialization / Shut Down
//---------------------------

eae6320::Math::cMatrix_transformation::cMatrix_transformation(
	const float i_00, const float i_10, const float i_20, const float i_30,
	const float i_01, const float i_11, const float i_21, const float i_31,
	const float i_02, const float i_12, const float i_22, const float i_32,
	const float i_03, const float i_13, const float i_23, const float i_33 )
	:
	m_00( i_00 ), m_10( i_10 ), m_20( i_20 ), m_30( i_30 ),
	m_01( i_01 ), m_11( i_11 ), m_21( i_21 ), m_31( i_31 ),
	m_02( i_02 ), m_12( i_12 ), m_22( i_22 ), m_32( i_32 ),
	m_03( i_03 ), m_13( i_13 ), m_23( i_23 ), m_33( i_33 )
{

}
