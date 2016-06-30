#ifndef MYHASH_H_
#define MYHASH_H_
#include <vector>
using namespace std;

class MyHash
{
  public:
	MyHash() : _numBuckets(0), _buckets(0) {}
	MyHash(size_t b) : _numBuckets(0), _buckets(0) { 
		init(b); 
	}
	~MyHash() { 
		reset(); 
	}

	void init(size_t b) {
		reset(); 
		_numBuckets = b; 
		_buckets = new vector<vector<int> >[b]; 
	}
	void reset() {
		_numBuckets = 0;
		if (_buckets) { 
			delete [] _buckets; 
			_buckets = 0; 
		}
	}
	size_t numBuckets() const { 
		return _numBuckets; 
	}

	// operator overloading
	vector<vector<int> >& operator [] (size_t i) { 
		return _buckets[i];
	}
	const vector<vector<int> >& operator [](size_t i) const { 
		return _buckets[i];
	}

	int check(int, unsigned, unsigned*);   


  private:
	size_t						_numBuckets;
	vector<vector<int> >*		_buckets;

	size_t bucketNum(const unsigned k) const {
		size_t sum= k;
		return (sum % _numBuckets); 
	}
};

#endif
