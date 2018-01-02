/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EfcDsrcApplication"
 * 	found in "IS_TS103301/ISO_TS_14906_Application.asn"
 * 	`asn1c -fcompound-names -fincludes-quoted -no-gen-example`
 */

#ifndef	_DieselEmissionValues_H_
#define	_DieselEmissionValues_H_


#include "asn_application.h"

/* Including external dependencies */
#include "Int2.h"
#include "UnitType.h"
#include "NativeInteger.h"
#include "constr_SEQUENCE.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DieselEmissionValues */
typedef struct DieselEmissionValues {
	struct DieselEmissionValues__particulate {
		UnitType_t	 unitType;
		long	 value;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} particulate;
	Int2_t	 absorptionCoeff;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DieselEmissionValues_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DieselEmissionValues;
extern asn_SEQUENCE_specifics_t asn_SPC_DieselEmissionValues_specs_1;
extern asn_TYPE_member_t asn_MBR_DieselEmissionValues_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _DieselEmissionValues_H_ */
#include "asn_internal.h"
