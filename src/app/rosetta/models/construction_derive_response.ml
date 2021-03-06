(*
 * This file has been generated by the OCamlClientCodegen generator for openapi-generator.
 *
 * Generated by: https://openapi-generator.tech
 *
 * Schema Construction_derive_response.t : ConstructionDeriveResponse is returned by the `/construction/derive` endpoint.
 *)

type t =
  { (* Address in network-specific format. *)
    address: string
  ; metadata: Yojson.Safe.t option [@default None] }
[@@deriving yojson {strict= false}, show]

(** ConstructionDeriveResponse is returned by the `/construction/derive` endpoint. *)
let create (address : string) : t = {address; metadata= None}
