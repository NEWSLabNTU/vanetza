/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "ElectronicRegistrationIdentificationVehicleDataModule"
 * 	found in "IS_TS103301/ISO_TS_24534-3.asn"
 * 	`asn1c -fcompound-names -fincludes-quoted -no-gen-example`
 */

#ifndef	_EuVehicleCategoryO_H_
#define	_EuVehicleCategoryO_H_


#include "asn_application.h"

/* Including external dependencies */
#include "NativeEnumerated.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EuVehicleCategoryO {
	EuVehicleCategoryO_o1	= 0,
	EuVehicleCategoryO_o2	= 1,
	EuVehicleCategoryO_o3	= 2,
	EuVehicleCategoryO_o4	= 3
} e_EuVehicleCategoryO;

/* EuVehicleCategoryO */
typedef long	 EuVehicleCategoryO_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_EuVehicleCategoryO_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_EuVehicleCategoryO;
extern const asn_INTEGER_specifics_t asn_SPC_EuVehicleCategoryO_specs_1;
asn_struct_free_f EuVehicleCategoryO_free;
asn_struct_print_f EuVehicleCategoryO_print;
asn_constr_check_f EuVehicleCategoryO_constraint;
ber_type_decoder_f EuVehicleCategoryO_decode_ber;
der_type_encoder_f EuVehicleCategoryO_encode_der;
xer_type_decoder_f EuVehicleCategoryO_decode_xer;
xer_type_encoder_f EuVehicleCategoryO_encode_xer;
oer_type_decoder_f EuVehicleCategoryO_decode_oer;
oer_type_encoder_f EuVehicleCategoryO_encode_oer;
per_type_decoder_f EuVehicleCategoryO_decode_uper;
per_type_encoder_f EuVehicleCategoryO_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _EuVehicleCategoryO_H_ */
#include "asn_internal.h"
