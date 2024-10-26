#pragma once
#include <istream>
#include "gDP.h"

class CombinerKey {
public:
	enum FastPath
	{
		// no optimizations applied
		CKFP_DISABLED,
		// in 1CYC passes without applying any blending
		// in 2CYC multiplies by fog
		CKFP_PASS,
		// in 1CYC multiplies by combined alpha
		// in 2CYC multiplies by fog, then multiplies by combined alpha
		CKFP_TRANSLUCENT,
		// in 1CYC reserved for future use
		// in 2CYC passes without applying any blending
		CKFP_PASS_TWICE,
	};

	CombinerKey() {
		m_key.mux = 0;
	}
	explicit CombinerKey(u64 _mux, bool _setModeBits = true);
	CombinerKey(const CombinerKey & _other);

	void operator=(u64 _mux);
	void operator=(const CombinerKey & _other);

	bool operator==(const CombinerKey & _other) const;
	bool operator<(const CombinerKey & _other) const;

	bool isRectKey() const;

	u32 getCycleType() const;

	u32 getBilerp() const;

	bool noAlpha() const;

	FastPath fastPath() const;

	u64 getMux() const { return m_key.mux; }

	void read(std::istream & _is);

	static const CombinerKey & getEmpty();

private:
	gDPCombine m_key;
};
