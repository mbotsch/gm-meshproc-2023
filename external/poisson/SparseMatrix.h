/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#ifndef __SPARSEMATRIX_HPP
#define __SPARSEMATRIX_HPP

#include "PoissonVector.h"
#include "Array.h"

template <class T>
struct MatrixEntry
{
	MatrixEntry( void )		    { N =-1; Value = 0; }
	MatrixEntry( int i )	    { N = i; Value = 0; }
	MatrixEntry( int i , T v )	{ N = i; Value = v; }
	int N;
	T Value;
};

template<class T> class SparseMatrix
{
private:
	bool _contiguous;
	int _maxEntriesPerRow;
	void _init( void );
public:
	int rows;
	Pointer( int ) rowSizes;
	Pointer( Pointer( MatrixEntry< T > ) ) m_ppElements;
	Pointer( MatrixEntry< T > ) operator[] ( int idx ) { return m_ppElements[idx]; }
	ConstPointer( MatrixEntry< T > ) operator[] ( int idx ) const { return m_ppElements[idx]; }

	SparseMatrix( void );
	SparseMatrix( int rows );
	SparseMatrix( int rows , int maxEntriesPerRow );
	void Resize( int rows );
	void Resize( int rows , int maxEntriesPerRow );
	void SetRowSize( int row , int count );
	int Entries( void ) const;

	SparseMatrix( const SparseMatrix& M );
	~SparseMatrix();

	void SetZero();
	void SetIdentity();

	SparseMatrix<T>& operator = (const SparseMatrix<T>& M);

	SparseMatrix<T> operator * (const T& V) const;
	SparseMatrix<T>& operator *= (const T& V);


	SparseMatrix<T> operator * (const SparseMatrix<T>& M) const;
	SparseMatrix<T> Multiply( const SparseMatrix<T>& M ) const;
	SparseMatrix<T> MultiplyTranspose( const SparseMatrix<T>& Mt ) const;

	template<class T2>
	PoissonVector<T2> operator * (const PoissonVector<T2>& V) const;
	template<class T2>
	PoissonVector<T2> Multiply( const PoissonVector<T2>& V ) const;
	template<class T2>
	void Multiply( const PoissonVector<T2>& In , PoissonVector<T2>& Out , int threads=1 ) const;


	SparseMatrix<T> Transpose() const;

	static int Solve			(const SparseMatrix<T>& M,const PoissonVector<T>& b, int iters,PoissonVector<T>& solution,const T eps=1e-8);

	template<class T2>
	static int SolveSymmetric( const SparseMatrix<T>& M , const PoissonVector<T2>& b , int iters , PoissonVector<T2>& solution , const T2 eps=1e-8 , int reset=1 , int threads=1 );

	bool write( FILE* fp ) const;
	bool write( const char* fileName ) const;
	bool read( FILE* fp );
	bool read( const char* fileName );
};


template< class T2 >
struct MapReducePoissonVector
{
private:
	int _dim;
public:
	std::vector< T2* > out;
	MapReducePoissonVector( void ) { _dim = 0; }
	~MapReducePoissonVector( void )
	{
		if( _dim ) for( int t=0 ; t<int(out.size()) ; t++ ) delete[] out[t];
		out.resize( 0 );
	}
	T2* operator[]( int t ) { return out[t]; }
	const T2* operator[]( int t ) const { return out[t]; }
	int threads( void ) const { return int( out.size() ); }
	void resize( int threads , int dim )
	{
                if( threads!=(int)out.size() || _dim<dim )
		{
			for( int t=0 ; t<int(out.size()) ; t++ ) delete[] out[t];
			out.resize( threads );
			for( int t=0 ; t<int(out.size()) ; t++ ) out[t] = new T2[dim];
			_dim = dim;
		}
	}

};

template< class T >
class SparseSymmetricMatrix : public SparseMatrix< T >
{
public:

	template< class T2 >
	PoissonVector< T2 > operator * ( const PoissonVector<T2>& V ) const;

	template< class T2 >
	PoissonVector< T2 > Multiply( const PoissonVector<T2>& V ) const;

	template< class T2 >
	void Multiply( const PoissonVector<T2>& In, PoissonVector<T2>& Out , bool addDCTerm=false ) const;

	template< class T2 >
	void Multiply( const PoissonVector<T2>& In, PoissonVector<T2>& Out , MapReducePoissonVector< T2 >& OutScratch , bool addDCTerm=false ) const;

	template< class T2 >
	void Multiply( const PoissonVector<T2>& In, PoissonVector<T2>& Out , std::vector< T2* >& OutScratch , const std::vector< int >& bounds ) const;

	template< class T2 >
	static int Solve( const SparseSymmetricMatrix<T>& M , const PoissonVector<T2>& b , int iters , PoissonVector<T2>& solution , T2 eps=1e-8 , int reset=1 , int threads=0  , bool addDCTerm=false , bool solveNormal=false );

	template< class T2 >
	static int Solve( const SparseSymmetricMatrix<T>& M , const PoissonVector<T2>& b , int iters , PoissonVector<T2>& solution , MapReducePoissonVector<T2>& scratch , T2 eps=1e-8 , int reset=1 , bool addDCTerm=false , bool solveNormal=false );
#ifdef WIN32
	template< class T2 >
	static int SolveAtomic( const SparseSymmetricMatrix<T>& M , const PoissonVector<T2>& b , int iters , PoissonVector<T2>& solution , T2 eps=1e-8 , int reset=1 , int threads=0  , bool solveNormal=false );
#endif // WIN32
	template<class T2>
	static int Solve( const SparseSymmetricMatrix<T>& M , const PoissonVector<T2>& diagonal , const PoissonVector<T2>& b , int iters , PoissonVector<T2>& solution , int reset=1 );

	template< class T2 >
	void getDiagonal( PoissonVector< T2 >& diagonal ) const;
};

#include "SparseMatrix.inl"

#endif

