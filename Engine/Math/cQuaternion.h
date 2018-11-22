/*
	This class represents a rotation or an orientation
*/

#ifndef EAE6320_MATH_CQUATERNION_H
#define EAE6320_MATH_CQUATERNION_H

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		struct sVector;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Math
	{
		class cQuaternion
		{
			// Interface
			//==========

		public:

			// Multiplication
			//---------------

			cQuaternion operator *( const cQuaternion i_rhs ) const;

			// Inversion
			//----------

			void Invert();
			cQuaternion GetInverse() const;

			// Normalization
			//--------------

			void Normalize();
			cQuaternion GetNormalized() const;

			// Products
			//---------

			friend float Dot( const cQuaternion i_lhs, const cQuaternion i_rhs );

			// Access
			//-------

			// Calculating the forward direction involves a variation of calculating a full transformation matrix;
			// if the transform is already available or will need to be calculated in the future
			// it is more efficient to extract the forward direction from that
			sVector CalculateForwardDirection() const;

			// Initialization / Shut Down
			//---------------------------

			cQuaternion() = default;	// Identity
			cQuaternion( const float i_angleInRadians,	// A positive angle rotates counter-clockwise (right-handed) around the axis
				const sVector i_axisOfRotation_normalized );

			// Data
			//=====

		private:

			float m_w = 1.0f;
			float m_x = 0.0f;
			float m_y = 0.0f;
			float m_z = 0.0f;

			// Implementation
			//===============

		private:

			// Initialization / Shut Down
			//---------------------------

			cQuaternion( const float i_w, const float i_x, const float i_y, const float i_z );

			// Friends
			//========

			friend class cMatrix_transformation;
		};

		// Friends
		//========

		float Dot( const cQuaternion i_lhs, const cQuaternion i_rhs );
	}
}

#endif	// EAE6320_MATH_CQUATERNION_H
