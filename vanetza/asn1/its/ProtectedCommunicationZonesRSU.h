/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "CDD_TS102894-2/ITS-Container.asn"
 * 	`asn1c -fcompound-names -fincludes-quoted -no-gen-example`
 */

#ifndef	_ProtectedCommunicationZonesRSU_H_
#define	_ProtectedCommunicationZonesRSU_H_


#include "asn_application.h"

/* Including external dependencies */
#include "asn_SEQUENCE_OF.h"
#include "constr_SEQUENCE_OF.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtectedCommunicationZone;

/* ProtectedCommunicationZonesRSU */
typedef struct ProtectedCommunicationZonesRSU {
	A_SEQUENCE_OF(struct ProtectedCommunicationZone) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtectedCommunicationZonesRSU_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtectedCommunicationZonesRSU;
extern asn_SET_OF_specifics_t asn_SPC_ProtectedCommunicationZonesRSU_specs_1;
extern asn_TYPE_member_t asn_MBR_ProtectedCommunicationZonesRSU_1[1];
extern asn_per_constraints_t asn_PER_type_ProtectedCommunicationZonesRSU_constr_1;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtectedCommunicationZone.h"

#endif	/* _ProtectedCommunicationZonesRSU_H_ */
#include "asn_internal.h"
