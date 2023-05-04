// © ABBYY. 2010.
// SAMPLES code is property of ABBYY, exclusive rights are reserved. 
//
// DEVELOPER is allowed to incorporate SAMPLES into his own APPLICATION and modify it under 
// the  terms of  License Agreement between  ABBYY and DEVELOPER.


// ABBYY FineReader Engine 10 Sample

// Smart pointer to any FineReader Engine interface.

#ifndef FineInterfacePtr_h
#define FineInterfacePtr_h

template<class T>
class CSafePtr {
public:
	// Constructors
    CSafePtr( T* _iface = 0 ) { attach( _iface ); }
    CSafePtr( const CSafePtr& other ) { attach( other.iface ); }

	// Destructor
    ~CSafePtr() { release(); }
    
	// Returns true if pointer is null
	bool IsNull() const { return iface == 0; }

	// Returns pointer to the internal pointer to the interface object
    T** GetBuffer();	
    void** GetQIBuffer();

	// Extractors
	T& operator *() const { return *iface; }
    T* operator ->() const { return iface; }
	T** operator &() { return GetBuffer(); }

	void CopyTo( T** result ) const;
	operator T*() const { return iface; }

	// Assigment operators
	CSafePtr& operator = ( const CSafePtr& other );
	CSafePtr& operator = ( T* _iface );

private:
	// Functions for internal use
	void attach( T* _iface );
	void release();

    T* iface;	    
};

template<class T>
inline T** CSafePtr<T>::GetBuffer()
{	
	release();
	return &iface;	
}


template<class T>
inline void** CSafePtr<T>::GetQIBuffer()
{	
	release();		
	return reinterpret_cast<void**>(&iface);	
}

template<class T>
inline CSafePtr<T>& CSafePtr<T>::operator = ( const CSafePtr<T>& other )
{
    if( other.iface != iface ) {
		release();
		attach( other.iface );	
    }
	return *this;
}

template<class T>
inline CSafePtr<T>& CSafePtr<T>::operator = ( T* _iface )
{
    if( _iface != iface ) {
		release();
		attach( _iface );	
    }
	return *this;
}

template<class T>
void CSafePtr<T>::CopyTo( T** result ) const
{
	*result = iface;
	if( (*result) != 0 ) {
		 (*result)->AddRef();
	}
}

template<class T>
inline void CSafePtr<T>::attach( T* _iface )
{	
	if( _iface != 0 ) {
		_iface->AddRef();	
	}
	iface = _iface;	
}

template<class T>
inline void CSafePtr<T>::release()
{	
	if( iface != 0 ) {
		iface->Release();	
	}
	iface = 0;	
}

#endif // FineInterfacePtr_h
