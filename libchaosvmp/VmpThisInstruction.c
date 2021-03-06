/*
 * 参数:
 *	bModrmByte:当前的MODRM字节
 *	pModRmTbl:转换后的MODRM表
 *
 * 介绍:
 *	使用转换后的MODRM表来重新编码MODRM字节,算法很简单通过pModRmTbl中转换后的每个位的位置来重新构造一个MODRM位
 */
__byte __INTERNAL_FUNC__ VmpModRm(__byte bModrmByte, __byte *pModRmTbl) {
	__byte bMOD = (bModrmByte & 0xC0) >> 6;
	__byte bRO = (bModrmByte & 0x38) >> 3;
	__byte bRM = bModrmByte & 0x07;
	__byte bModrm = 0;
	__integer i = 0;
	__dword dwLocal = 0;
	__logic_memcpy__((__memory)&dwLocal, pModRmTbl, sizeof(__byte) * 3);
	switch (dwLocal) {
		case 0x0102:{//"\x00\x01\x02"
			bModrm = bModrmByte;
		}break;
		case 0x0201:{//"\x00\x02\x01"
			bMOD = (bModrmByte & 0xC0) >> 6;
			bRO = (bModrmByte & 0x38) >> 3;
			bRM = (bModrmByte & 0x07);
			bModrm |= (bMOD << 6);
			bModrm |= (bRO);
			bModrm |= (bRM << 3);
		}break;
		case 0x010002:{//"\x01\x00\x02"
			bMOD = (bModrmByte & 0xC0) >> 6;
			bRO = (bModrmByte & 0x38) >> 3;
			bRM = (bModrmByte & 0x07);
			bModrm |= (bMOD << 3);
			bModrm |= (bRO << 5);
			bModrm |= (bRM);
		}break;
		case 0x010200:{//"\x01\x02\x00"
			bMOD = (bModrmByte & 0xC0) >> 6;
			bRO = (bModrmByte & 0x38) >> 3;
			bRM = (bModrmByte & 0x07);
			bModrm |= (bMOD);
			bModrm |= (bRO << 5);
			bModrm |= (bRM << 2);
		}break;
		case 0x020001:{//"\x02\x00\x01"
			bMOD = (bModrmByte & 0xC0) >> 6;
			bRO = (bModrmByte & 0x38) >> 3;
			bRM = (bModrmByte & 0x07);
			bModrm |= (bMOD << 3);
			bModrm |= (bRO);
			bModrm |= (bRM << 5);
		}break;
		case 0x020100:{//"\x02\x01\x00"
			bMOD = (bModrmByte & 0xC0) >> 6;
			bRO = (bModrmByte & 0x38) >> 3;
			bRM = (bModrmByte & 0x07);
			bModrm |= (bMOD);
			bModrm |= (bRO << 2);
			bModrm |= (bRM << 5);
		}break;
	}

	return bModrm;
}

/*
 * 参数:
 *	bOrigSib:当前的SIB字节
 *	pSibTbl:转换后的SIB表
 *
 * 介绍:
 *	使用转换后的SIB表来重新编码SIB字节,算法很简单通过pSibTbl中转换后的每个位的位置来重新构造一个SIB位
 */
__byte __INTERNAL_FUNC__ VmpSib(__byte bOrigSib, __byte *pSibTbl) {
	__byte bSS = (bOrigSib & 0xC0) >> 6;
	__byte bIndex = (bOrigSib & 0x38) >> 3;
	__byte bBase = bOrigSib & 0x07;
	__byte bSib = 0;
	__integer i = 0;
	__dword dwLocal = 0;
	__logic_memcpy__((__memory)&dwLocal, pSibTbl, sizeof(__byte) * 3);
	switch (dwLocal) {
		case 0x0102:{//"\x00\x01\x02"
			bSib = bOrigSib;
		}break;
		case 0x0201:{//"\x00\x02\x01"
			bSS = (bOrigSib & 0xC0) >> 6;
			bIndex = (bOrigSib & 0x38) >> 3;
			bBase = (bOrigSib & 0x07);
			bSib |= (bSS << 6);
			bSib |= (bIndex);
			bSib |= (bBase << 3);
		}break;
		case 0x010002:{//"\x01\x00\x02"
			bSS = (bOrigSib & 0xC0) >> 6;
			bIndex = (bOrigSib & 0x38) >> 3;
			bBase = (bOrigSib & 0x07);
			bSib |= (bSS << 3);
			bSib |= (bIndex << 5);
			bSib |= (bBase);
		}break;
		case 0x010200:{//"\x01\x02\x00"
			bSS = (bOrigSib & 0xC0) >> 6;
			bIndex = (bOrigSib & 0x38) >> 3;
			bBase = (bOrigSib & 0x07);
			bSib |= (bSS);
			bSib |= (bIndex << 5);
			bSib |= (bBase << 2);
		}break;
		case 0x020001:{//"\x02\x00\x01"
			bSS = (bOrigSib & 0xC0) >> 6;
			bIndex = (bOrigSib & 0x38) >> 3;
			bBase = (bOrigSib & 0x07);
			bSib |= (bSS << 3);
			bSib |= (bIndex);
			bSib |= (bBase << 5);
		}break;
		case 0x020100:{//"\x02\x01\x00"
			bSS = (bOrigSib & 0xC0) >> 6;
			bIndex = (bOrigSib & 0x38) >> 3;
			bBase = (bOrigSib & 0x07);
			bSib |= (bSS);
			bSib |= (bIndex << 2);
			bSib |= (bBase << 5);
		}break;
	}

	return bSib;
}

/*
 * 参数:
 *	bModRm:当前的MODRM字节
 *
 * 介绍:
 *	通过当前的MODRM编码来确定这条指令是否存在SIB位
 */
__bool __INTERNAL_FUNC__ HasSib(__byte bModRm) {
	__byte bMod = (bModRm & 0xC0) >> 6;
	__byte bRo = (bModRm & 0x38) >> 3;
	__byte bRm = bModRm & 0x07;

	if (bMod == 3) return FALSE;

	if (bRm == 4) return TRUE;

	return FALSE;
}

/*
 * 参数:
 *	pBuf:要设置MODRM/SIB位的指针位置
 * pModRmTbl:转换后的MODRM表
 * pSibTbl:转换后的SIB表
 *
 * 介绍:
 *	重新编码指令的MODRM和SIB字节,并返回MODRM与SIB的总和个数
 */
__INLINE__ __integer __INTERNAL_FUNC__ VmpModrmSib(__memory pBuf, __byte *pModRmTbl, __byte *pSibTbl) {
	__integer iCount = 0;
	__byte bByte = *pBuf;
	*(pBuf + iCount) = VmpModRm(bByte, pModRmTbl);iCount++;
	if (HasSib(bByte)) {//如果拥有SIB位则编码Sib
		bByte = *(pBuf + iCount);
		*(pBuf + iCount) = VmpSib(bByte, pSibTbl);iCount++;
	}
	return iCount;
}

/*
 * 参数:
 *	pud_obj:当前指令的反汇编结构
 *	pInst:指向指令当前的文件地址
 *	iInstLen:指令的长度
 *	pOpcode1Tbl:被转换后的第一张编码表
 *	pOpcode2Tbl:被转换后的第二张编码表
 *	pModRmTbl:被转换后的MODRM表
 *	pSibTbl:被转换后的SIB表
 *
 *	介绍:
 *	虚拟编码每条指令所用的函数集,对每一个支持的指令都有一个函数与它对应,返回这条指令除了指令部分+MODRM/SIB长度之后的长度
 */

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iaaa(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	*pInst = *(pOpcode1Tbl + bByte);
	return iInstLen - 1;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iaad(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	*pInst = *(pOpcode1Tbl + bByte);
	return iInstLen - 1;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iaam(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	*pInst = *(pOpcode1Tbl + bByte);
	return iInstLen - 1;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iaas(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	*pInst = *(pOpcode1Tbl + bByte);
	return iInstLen - 1;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iadc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	if ((bByte == 0x14) || (bByte == 0x15)) {
		*pInst = *(pOpcode1Tbl + bByte);
		iInstLen--;
	} else {
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}

	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iadd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	if ((bByte == 0x04) || (bByte == 0x05)) {
		*pInst = *(pOpcode1Tbl + bByte);
		iInstLen--;
	} else {
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}

	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iand(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	if ((bByte == 0x24) || (bByte == 0x25)) {
		*pInst = *(pOpcode1Tbl + bByte);
		iInstLen--;
	} else {
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}

	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iarpl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibound(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibsf(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibsr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibswap(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	if (bByte == 0xA3) {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibtc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	if (bByte == 0xBB) {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibtr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	if (bByte == 0xB3) {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ibts(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	if (bByte == 0xAB) {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icall(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0xE8) || (bByte == 0x9A)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icbw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icwde(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iclc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icld(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iclflush(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icli(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iclts(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovo(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovno(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovae(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovnz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovbe(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmova(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovs(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovns(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovnp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovge(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovle(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmovg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	if ((bByte == 0x3C) || (bByte == 0x3D)) {
		*pInst = *(pOpcode1Tbl + bByte);
		iInstLen--;
	} else {
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}

	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmpsb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmpsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmpsd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmpxchg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icmpxchg8b(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icpuid(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icwd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Icdq(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Idaa(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Idas(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Idec(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0xFE) || (bByte == 0xFF)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Idiv(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iemms(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ienter(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ihlt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iidiv(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iin(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iimul(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte != 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	}
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinsb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinsd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iint1(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iint3(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iint(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinto(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinvd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iinvlpg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iiretw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iiretd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijo(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijno(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijae(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijnz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijbe(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ija(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijs(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijns(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijnp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijge(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijle(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijcxz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijecxz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ijmp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte != 0xFF) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {//0xFF
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilahf(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilar(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ildmxcsr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilds(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilea(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iles(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilfs(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilgs(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilidt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilss(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ileave(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilfence(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilgdt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Illdt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilmsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilodsb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilodsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilodsd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iloopnz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iloope(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iloop(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ilsl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iltr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imfence(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imonitor(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imov(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0x88) || (bByte == 0x89) || (bByte == 0x8A) || \
		(bByte == 0x8B) || (bByte == 0x8C) || (bByte == 0x8E) || \
		(bByte == 0xC6) || (bByte == 0xC7)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imovsb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imovsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imovsd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imovsx(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imovzx(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imul(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Imwait(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ineg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Inop(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x90) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Inot(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ior(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0x0C) || (bByte == 0x0D)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iout(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ioutsb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ioutsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ioutsd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipause(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipop(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0x8F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipopa(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipopad(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipopfw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipopfd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iprefetch(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iprefetchnta(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iprefetcht0(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iprefetcht1(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iprefetcht2(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipush(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if (bByte == 0xFF) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else if (bByte == 0x0F) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipusha(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipushad(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipushfw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ipushfd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ircl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ircr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irol(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iror(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irdmsr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irdpmc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irdtsc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irdtscp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iret(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Irsm(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isahf(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isal(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isar(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ishl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ishr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isbb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0x1C) || (bByte == 0x1D)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iscasb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iscasw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iscasd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iseto(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetno(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetnb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetnz(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetbe(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iseta(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isets(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetns(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetnp(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetl(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetge(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetle(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isetg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isfence(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isgdt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ishld(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ishrd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isidt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isldt(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ismsw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istc(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isti(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istmxcsr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istosb(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istosw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istosd(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Istr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isub(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0x2C) || (bByte == 0x2D)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isyscall(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isysenter(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isysexit(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Isysret(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Itest(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0xA8) || (bByte == 0xA9)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iverr(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iverw(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode2Tbl + bByte);iCount++;iInstLen--;
	iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Iwait(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ixchg(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	bByte = *(pInst + iCount);
	if ((bByte == 0x86) || (bByte == 0x87)) {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	} else {
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
	}
	return iInstLen;
}

__INLINE__ __integer __INTERNAL_FUNC__ VmpThisInstruction_Ixor(ud_t *pud_obj, __memory pInst, __integer iInstLen, __byte *pOpcode1Tbl, __byte *pOpcode2Tbl, __byte *pModRmTbl, __byte *pSibTbl) {
	__byte bByte = *pInst;
	__byte iCount = 0;
	if ((bByte == 0x34) || (bByte == 0x35)) {
		*pInst = *(pOpcode1Tbl + bByte);
		iInstLen--;
	} else {
		bByte = *(pInst + iCount);
		*(pInst + iCount) = *(pOpcode1Tbl + bByte);iCount++;iInstLen--;
		iInstLen -= VmpModrmSib(pInst + iCount, pModRmTbl, pSibTbl);
	}

	return iInstLen;
}
