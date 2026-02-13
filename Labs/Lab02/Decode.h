#ifndef DECODE_H
#define DECODE_H

#include "IAS.h"
#include <string>

namespace osl
{
	class Decode
	{
		public:
		static void execute(IAS& ias)
		{
			std::string ir = ias(1).read();
			while(ir.length() < 8) ir = "0" + ir;
			int op = osl::toDecimal(ir);
			if(op < 0) return;
			switch(op) {
				case 0x00: HLT(ias); break;
				case 0x01: LDA(ias); break;
				case 0x02: LDN(ias); break;
				case 0x03: ALD(ias); break;
				case 0x04: ALN(ias); break;
				case 0x05: ADD(ias); break;
				case 0x06: SUB(ias); break;
				case 0x07: AAD(ias); break;
				case 0x08: ASB(ias); break;
				case 0x09: LDM(ias); break;
				case 0x0A: LMA(ias); break;
				case 0x0B: MUL(ias); break;
				case 0x0C: DIV(ias); break;
				case 0x0D: BRL(ias); break;
				case 0x0E: BRR(ias); break;
				case 0x0F: BPL(ias); break;
				case 0x10: BPR(ias); break;
				case 0x12: STL(ias); break;
				case 0x13: STR(ias); break;
				case 0x14: LSH(ias); break;
				case 0x15: RSH(ias); break;
				case 0x21: STA(ias); break;
			}
		}

		private:
		static int addr(IAS& ias) {
			std::string mar = ias(3).read();
			while(mar.length() < 12) mar = "0" + mar;
			return osl::toDecimal(mar);
		}
		static std::string pad40(std::string s) {
			while(s.length() < 40) s = "0" + s;
			return s.length() > 40 ? s.substr(s.length() - 40, 40) : s;
		}
		static bool inRange(int x) { return x >= 0 && x < 4096; }
		static std::string abs40(std::string s) {
			s = pad40(s);
			return (s.length() > 0 && s[0] == '1') ? osl::negate(s) : s;
		}
		static void add40(IAS& ias, const std::string& b) {
			std::string a = pad40(ias(5).read());
			bool carry = false;
			std::string res(40, '0');
			for(int i = 39; i >= 0; i--) {
				int sum = (a[i]=='1') + (b[i]=='1') + carry;
				res[i] = (sum & 1) ? '1' : '0';
				carry = (sum >= 2);
			}
			ias(5).write(res);
		}
		static void branch(IAS& ias, bool ifNonNeg, bool left) {
			std::string ac = pad40(ias(5).read());
			if(ifNonNeg && ac.length() > 0 && ac[0] == '1') return;
			int x = addr(ias);
			if(!inRange(x)) return;
			std::string w = pad40(ias[x].read());
			ias(2).write(left ? w.substr(0, 20) : w.substr(20, 20));
		}
		static void store12(IAS& ias, int x, int pos) {
			std::string part = pad40(ias(5).read()).substr(28, 12);
			std::string cell = pad40(ias[x].read());
			ias[x].write(pos + 12 >= 40 ? cell.substr(0, pos) + part : cell.substr(0, pos) + part + cell.substr(pos + 12));
		}

		static void LMA(IAS& ias)  { ias(5).write(pad40(ias(6).read())); }
		static void LDM(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias(6).write(pad40(ias[x].read())); }
		static void STA(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias[x].write(pad40(ias(5).read())); }
		static void LDA(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias(5).write(pad40(ias[x].read())); }
		static void LDN(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias(5).write(pad40(osl::negate(pad40(ias[x].read())))); }
		static void ALD(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias(5).write(pad40(abs40(ias[x].read()))); }
		static void ALN(IAS& ias)  { int x = addr(ias); if(inRange(x)) ias(5).write(pad40(osl::negate(abs40(ias[x].read())))); }
		static void BRL(IAS& ias)  { branch(ias, false, true); }
		static void BRR(IAS& ias)  { branch(ias, false, false); }
		static void BPL(IAS& ias)  { branch(ias, true, true); }
		static void BPR(IAS& ias)  { branch(ias, true, false); }
		static void ADD(IAS& ias)  { int x = addr(ias); if(inRange(x)) add40(ias, pad40(ias[x].read())); }
		static void SUB(IAS& ias)  { int x = addr(ias); if(inRange(x)) add40(ias, osl::negate(pad40(ias[x].read()))); }
		static void AAD(IAS& ias)  { int x = addr(ias); if(inRange(x)) add40(ias, abs40(ias[x].read())); }
		static void ASB(IAS& ias)  { int x = addr(ias); if(inRange(x)) add40(ias, osl::negate(abs40(ias[x].read()))); }
		static void LSH(IAS& ias)  { std::string ac = pad40(ias(5).read()); ias(5).write(pad40(ac.substr(1, 39) + "0")); }
		static void RSH(IAS& ias)  { std::string ac = pad40(ias(5).read()); ias(5).write(pad40(std::string(1, ac[0]) + ac.substr(0, 39))); }
		static void STL(IAS& ias)  { int x = addr(ias); if(inRange(x)) store12(ias, x, 8); }
		static void STR(IAS& ias)  { int x = addr(ias); if(inRange(x)) store12(ias, x, 28); }
		static void HLT(IAS& ias)  { (void)ias; }

		static void MUL(IAS& ias)
		{
			int x = addr(ias);
			if(!inRange(x)) return;
			std::string mq = pad40(ias(6).read()), mx = pad40(ias[x].read());
			long long a = 0, b = 0;
			for(int i = 0; i < 40; i++) { a = (a << 1) | (mq[i]=='1'); b = (b << 1) | (mx[i]=='1'); }
			if(mq[0]=='1') a -= (1LL<<40);
			if(mx[0]=='1') b -= (1LL<<40);
			unsigned long long u = static_cast<unsigned long long>(a * b);
			std::string lo, hi;
			for(int i = 0; i < 40; i++) { lo = (u&1 ? "1":"0") + lo; u >>= 1; }
			for(int i = 0; i < 40; i++) { hi = (u&1 ? "1":"0") + hi; u >>= 1; }
			ias(5).write(pad40(hi));
			ias(6).write(pad40(lo));
		}
		static void DIV(IAS& ias)
		{
			int x = addr(ias);
			if(!inRange(x)) return;
			std::string ac = pad40(ias(5).read()), mx = pad40(ias[x].read());
			long long dv = 0, ds = 0;
			for(int i = 0; i < 40; i++) { dv = (dv<<1) | (ac[i]=='1'); ds = (ds<<1) | (mx[i]=='1'); }
			if(ac[0]=='1') dv -= (1LL<<40);
			if(mx[0]=='1') ds -= (1LL<<40);
			if(ds == 0) return;
			unsigned long long uq = static_cast<unsigned long long>(dv/ds), ur = static_cast<unsigned long long>(dv%ds);
			std::string qs, rs;
			for(int i = 0; i < 40; i++) { qs = (uq&1 ? "1":"0") + qs; uq >>= 1; }
			for(int i = 0; i < 40; i++) { rs = (ur&1 ? "1":"0") + rs; ur >>= 1; }
			ias(6).write(pad40(qs));
			ias(5).write(pad40(rs));
		}
	};
}

#endif
