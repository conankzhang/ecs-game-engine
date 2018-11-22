// Includes
//=========

#include "cQuaternion.h"

#include "sVector.h"

#include <cmath>
#include <Engine/Asserts/Asserts.h>

// Static Data Initialization
//===========================

namespace
{
	constexpr auto s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Multiplication
//---------------

eae6320::Math::cQuaternion eae6320::Math::cQuaternion::operator *( const cQuaternion i_rhs ) const
{
	return cQuaternion(
		( m_w * i_rhs.m_w ) - ( ( m_x * i_rhs.m_x ) + ( m_y * i_rhs.m_y ) + ( m_z * i_rhs.m_z ) ),
		( m_w * i_rhs.m_x ) + ( m_x * i_rhs.m_w ) + ( ( m_y * i_rhs.m_z ) - ( m_z * i_rhs.m_y ) ),
		( m_w * i_rhs.m_y ) + ( m_y * i_rhs.m_w ) + ( ( m_z * i_rhs.m_x ) - ( m_x * i_rhs.m_z ) ),
		( m_w * i_rhs.m_z ) + ( m_z * i_rhs.m_w ) + ( ( m_x * i_rhs.m_y ) - ( m_y * i_rhs.m_x ) ) );
}

// Inversion
//----------

void eae6320::Math::cQuaternion::Invert()
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;
}

eae6320::Math::cQuaternion eae6320::Math::cQuaternion::GetInverse() const
{
	return cQuaternion( m_w, -m_x, -m_y, -m_z );
}

// Normalization
//--------------

void eae6320::Math::cQuaternion::Normalize()
{
	const auto length = std::sqrt( ( m_w * m_w ) + ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
	EAE6320_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	const auto length_reciprocal = 1.0f / length;
	m_w *= length_reciprocal;
	m_x *= length_reciprocal;
	m_y *= length_reciprocal;
	m_z *= length_reciprocal;
}

eae6320::Math::cQuaternion eae6320::Math::cQuaternion::GetNormalized() const
{
	const auto length = std::sqrt( ( m_w * m_w ) + ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
	EAE6320_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	const auto length_reciprocal = 1.0f / length;
	return cQuaternion( m_w * length_reciprocal, m_x * length_reciprocal, m_y * length_reciprocal, m_z * length_reciprocal );
}

// Products
//---------

float eae6320::Math::Dot( const cQuaternion i_lhs, const cQuaternion i_rhs )
{
	return ( i_lhs.m_w * i_rhs.m_w ) + ( i_lhs.m_x * i_rhs.m_x ) + ( i_lhs.m_y * i_rhs.m_y ) + ( i_lhs.m_z * i_rhs.m_z );
}

// Access
//-------

eae6320::Math::sVector eae6320::Math::cQuaternion::CalculateForwardDirection() const
{
	const auto _2x = m_x + m_x;
	const auto _2y = m_y + m_y;
	const auto _2xx = m_x * _2x;
	const auto _2xz = _2x * m_z;
	const auto _2xw = _2x * m_w;
	const auto _2yy = _2y * m_y;
	const auto _2yz = _2y * m_z;
	const auto _2yw = _2y * m_w;

	return sVector( -_2xz - _2yw, -_2yz + _2xw, -1.0f + _2xx + _2yy );
}

// Initialization / Shut Down
//---------------------------

eae6320::Math::cQuaternion::cQuaternion( const float i_angleInRadians, const sVector i_axisOfRotation_normalized )
{
	const auto theta_half = i_angleInRadians * 0.5f;
	m_w = std::cos( theta_half );
	const auto sin_theta_half = std::sin( theta_half );
	m_x = i_axisOfRotation_normalized.x * sin_theta_half;
	m_y = i_axisOfRotation_normalized.y * sin_theta_half;
	m_z = i_axisOfRotation_normalized.z * sin_theta_half;
}

// Implementation
//===============

// Initialization / Shut Down
//---------------------------

eae6320::Math::cQuaternion::cQuaternion( const float i_w, const float i_x, const float i_y, const float i_z )
	:
	m_w( i_w ), m_x( i_x ), m_y( i_y ), m_z( i_z )
{

}
