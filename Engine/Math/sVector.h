/*
	This struct represents a position or direction
*/

#ifndef EAE6320_MATH_SVECTOR_H
#define EAE6320_MATH_SVECTOR_H

// Struct Declaration
//===================

namespace eae6320
{
	namespace Math
	{
		struct sVector
		{
			// Data
			//=====

			float x = 0.0f, y = 0.0f, z = 0.0f;

			// Interface
			//==========

			// Addition
			//---------

			sVector operator +( const sVector i_rhs ) const;
			sVector& operator +=( const sVector i_rhs );

			// Subtraction / Negation
			//-----------------------

			sVector operator -( const sVector i_rhs ) const;
			sVector& operator -=( const sVector i_rhs );
			sVector operator -() const;

			// Multiplication
			//---------------

			sVector operator *( const float i_rhs ) const;
			sVector& operator *=( const float i_rhs );
			friend sVector operator *( const float i_lhs, const sVector i_rhs );

			// Division
			//---------

			sVector operator /( const float i_rhs ) const;
			sVector& operator /=( const float i_rhs );

			// Length / Normalization
			//-----------------------

			float GetLength() const;
			float Normalize();
			sVector GetNormalized() const;

			// Products
			//---------

			friend float Dot( const sVector i_lhs, const sVector i_rhs );
			friend sVector Cross( const sVector i_lhs, const sVector i_rhs );

			// Comparison
			//-----------

			bool operator ==( const sVector i_rhs ) const;
			bool operator !=( const sVector i_rhs ) const;

			// Initialization / Shut Down
			//---------------------------

			sVector() = default;
			sVector( const float i_x, const float i_y, const float i_z );
		};

		// Friends
		//========

		sVector operator *( const float i_lhs, const sVector i_rhs );
		float Dot( const sVector i_lhs, const sVector i_rhs );
		sVector Cross( const sVector i_lhs, const sVector i_rhs );
	}
}

#endif	// EAE6320_MATH_SVECTOR_H
