// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FileSingleHeader.proto

#ifndef PROTOBUF_FileSingleHeader_2eproto__INCLUDED
#define PROTOBUF_FileSingleHeader_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace QDLIB {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_FileSingleHeader_2eproto();
void protobuf_AssignDesc_FileSingleHeader_2eproto();
void protobuf_ShutdownFile_FileSingleHeader_2eproto();

class FileSingleHeader;
class MultiStateIdentifier;

enum FileSingleHeader_Compression {
  FileSingleHeader_Compression_UNCOMPRESSED = 0,
  FileSingleHeader_Compression_ZLIB = 1,
  FileSingleHeader_Compression_BZIP = 2
};
bool FileSingleHeader_Compression_IsValid(int value);
const FileSingleHeader_Compression FileSingleHeader_Compression_Compression_MIN = FileSingleHeader_Compression_UNCOMPRESSED;
const FileSingleHeader_Compression FileSingleHeader_Compression_Compression_MAX = FileSingleHeader_Compression_BZIP;
const int FileSingleHeader_Compression_Compression_ARRAYSIZE = FileSingleHeader_Compression_Compression_MAX + 1;

const ::google::protobuf::EnumDescriptor* FileSingleHeader_Compression_descriptor();
inline const ::std::string& FileSingleHeader_Compression_Name(FileSingleHeader_Compression value) {
  return ::google::protobuf::internal::NameOfEnum(
    FileSingleHeader_Compression_descriptor(), value);
}
inline bool FileSingleHeader_Compression_Parse(
    const ::std::string& name, FileSingleHeader_Compression* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FileSingleHeader_Compression>(
    FileSingleHeader_Compression_descriptor(), name, value);
}
// ===================================================================

class FileSingleHeader : public ::google::protobuf::Message {
 public:
  FileSingleHeader();
  virtual ~FileSingleHeader();
  
  FileSingleHeader(const FileSingleHeader& from);
  
  inline FileSingleHeader& operator=(const FileSingleHeader& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const FileSingleHeader& default_instance();
  
  void Swap(FileSingleHeader* other);
  
  // implements Message ----------------------------------------------
  
  FileSingleHeader* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FileSingleHeader& from);
  void MergeFrom(const FileSingleHeader& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  typedef FileSingleHeader_Compression Compression;
  static const Compression UNCOMPRESSED = FileSingleHeader_Compression_UNCOMPRESSED;
  static const Compression ZLIB = FileSingleHeader_Compression_ZLIB;
  static const Compression BZIP = FileSingleHeader_Compression_BZIP;
  static inline bool Compression_IsValid(int value) {
    return FileSingleHeader_Compression_IsValid(value);
  }
  static const Compression Compression_MIN =
    FileSingleHeader_Compression_Compression_MIN;
  static const Compression Compression_MAX =
    FileSingleHeader_Compression_Compression_MAX;
  static const int Compression_ARRAYSIZE =
    FileSingleHeader_Compression_Compression_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Compression_descriptor() {
    return FileSingleHeader_Compression_descriptor();
  }
  static inline const ::std::string& Compression_Name(Compression value) {
    return FileSingleHeader_Compression_Name(value);
  }
  static inline bool Compression_Parse(const ::std::string& name,
      Compression* value) {
    return FileSingleHeader_Compression_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // required bool more_files_follow = 1 [default = false];
  inline bool has_more_files_follow() const;
  inline void clear_more_files_follow();
  static const int kMoreFilesFollowFieldNumber = 1;
  inline bool more_files_follow() const;
  inline void set_more_files_follow(bool value);
  
  // required string class = 2;
  inline bool has_class_() const;
  inline void clear_class_();
  static const int kClassFieldNumber = 2;
  inline const ::std::string& class_() const;
  inline void set_class_(const ::std::string& value);
  inline void set_class_(const char* value);
  inline void set_class_(const char* value, size_t size);
  inline ::std::string* mutable_class_();
  inline ::std::string* release_class_();
  
  // optional string name = 3;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 3;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // optional uint32 sequence_number = 4;
  inline bool has_sequence_number() const;
  inline void clear_sequence_number();
  static const int kSequenceNumberFieldNumber = 4;
  inline ::google::protobuf::uint32 sequence_number() const;
  inline void set_sequence_number(::google::protobuf::uint32 value);
  
  // required string meta_data = 5;
  inline bool has_meta_data() const;
  inline void clear_meta_data();
  static const int kMetaDataFieldNumber = 5;
  inline const ::std::string& meta_data() const;
  inline void set_meta_data(const ::std::string& value);
  inline void set_meta_data(const char* value);
  inline void set_meta_data(const char* value, size_t size);
  inline ::std::string* mutable_meta_data();
  inline ::std::string* release_meta_data();
  
  // required uint64 payload_size = 6 [default = 0];
  inline bool has_payload_size() const;
  inline void clear_payload_size();
  static const int kPayloadSizeFieldNumber = 6;
  inline ::google::protobuf::uint64 payload_size() const;
  inline void set_payload_size(::google::protobuf::uint64 value);
  
  // required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
  inline bool has_compression() const;
  inline void clear_compression();
  static const int kCompressionFieldNumber = 7;
  inline ::QDLIB::FileSingleHeader_Compression compression() const;
  inline void set_compression(::QDLIB::FileSingleHeader_Compression value);
  
  // optional bytes sub_header = 10;
  inline bool has_sub_header() const;
  inline void clear_sub_header();
  static const int kSubHeaderFieldNumber = 10;
  inline const ::std::string& sub_header() const;
  inline void set_sub_header(const ::std::string& value);
  inline void set_sub_header(const char* value);
  inline void set_sub_header(const void* value, size_t size);
  inline ::std::string* mutable_sub_header();
  inline ::std::string* release_sub_header();
  
  // @@protoc_insertion_point(class_scope:QDLIB.FileSingleHeader)
 private:
  inline void set_has_more_files_follow();
  inline void clear_has_more_files_follow();
  inline void set_has_class_();
  inline void clear_has_class_();
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_sequence_number();
  inline void clear_has_sequence_number();
  inline void set_has_meta_data();
  inline void clear_has_meta_data();
  inline void set_has_payload_size();
  inline void clear_has_payload_size();
  inline void set_has_compression();
  inline void clear_has_compression();
  inline void set_has_sub_header();
  inline void clear_has_sub_header();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* class__;
  bool more_files_follow_;
  ::google::protobuf::uint32 sequence_number_;
  ::std::string* name_;
  ::std::string* meta_data_;
  ::google::protobuf::uint64 payload_size_;
  ::std::string* sub_header_;
  int compression_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(8 + 31) / 32];
  
  friend void  protobuf_AddDesc_FileSingleHeader_2eproto();
  friend void protobuf_AssignDesc_FileSingleHeader_2eproto();
  friend void protobuf_ShutdownFile_FileSingleHeader_2eproto();
  
  void InitAsDefaultInstance();
  static FileSingleHeader* default_instance_;
};
// -------------------------------------------------------------------

class MultiStateIdentifier : public ::google::protobuf::Message {
 public:
  MultiStateIdentifier();
  virtual ~MultiStateIdentifier();
  
  MultiStateIdentifier(const MultiStateIdentifier& from);
  
  inline MultiStateIdentifier& operator=(const MultiStateIdentifier& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const MultiStateIdentifier& default_instance();
  
  void Swap(MultiStateIdentifier* other);
  
  // implements Message ----------------------------------------------
  
  MultiStateIdentifier* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MultiStateIdentifier& from);
  void MergeFrom(const MultiStateIdentifier& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QDLIB.MultiStateIdentifier)
 private:
  inline void set_has_state();
  inline void clear_has_state();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 state_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_FileSingleHeader_2eproto();
  friend void protobuf_AssignDesc_FileSingleHeader_2eproto();
  friend void protobuf_ShutdownFile_FileSingleHeader_2eproto();
  
  void InitAsDefaultInstance();
  static MultiStateIdentifier* default_instance_;
};
// ===================================================================


// ===================================================================

// FileSingleHeader

// required bool more_files_follow = 1 [default = false];
inline bool FileSingleHeader::has_more_files_follow() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FileSingleHeader::set_has_more_files_follow() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FileSingleHeader::clear_has_more_files_follow() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FileSingleHeader::clear_more_files_follow() {
  more_files_follow_ = false;
  clear_has_more_files_follow();
}
inline bool FileSingleHeader::more_files_follow() const {
  return more_files_follow_;
}
inline void FileSingleHeader::set_more_files_follow(bool value) {
  set_has_more_files_follow();
  more_files_follow_ = value;
}

// required string class = 2;
inline bool FileSingleHeader::has_class_() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void FileSingleHeader::set_has_class_() {
  _has_bits_[0] |= 0x00000002u;
}
inline void FileSingleHeader::clear_has_class_() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void FileSingleHeader::clear_class_() {
  if (class__ != &::google::protobuf::internal::kEmptyString) {
    class__->clear();
  }
  clear_has_class_();
}
inline const ::std::string& FileSingleHeader::class_() const {
  return *class__;
}
inline void FileSingleHeader::set_class_(const ::std::string& value) {
  set_has_class_();
  if (class__ == &::google::protobuf::internal::kEmptyString) {
    class__ = new ::std::string;
  }
  class__->assign(value);
}
inline void FileSingleHeader::set_class_(const char* value) {
  set_has_class_();
  if (class__ == &::google::protobuf::internal::kEmptyString) {
    class__ = new ::std::string;
  }
  class__->assign(value);
}
inline void FileSingleHeader::set_class_(const char* value, size_t size) {
  set_has_class_();
  if (class__ == &::google::protobuf::internal::kEmptyString) {
    class__ = new ::std::string;
  }
  class__->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FileSingleHeader::mutable_class_() {
  set_has_class_();
  if (class__ == &::google::protobuf::internal::kEmptyString) {
    class__ = new ::std::string;
  }
  return class__;
}
inline ::std::string* FileSingleHeader::release_class_() {
  clear_has_class_();
  if (class__ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = class__;
    class__ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string name = 3;
inline bool FileSingleHeader::has_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void FileSingleHeader::set_has_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void FileSingleHeader::clear_has_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void FileSingleHeader::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& FileSingleHeader::name() const {
  return *name_;
}
inline void FileSingleHeader::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void FileSingleHeader::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void FileSingleHeader::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FileSingleHeader::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* FileSingleHeader::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional uint32 sequence_number = 4;
inline bool FileSingleHeader::has_sequence_number() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void FileSingleHeader::set_has_sequence_number() {
  _has_bits_[0] |= 0x00000008u;
}
inline void FileSingleHeader::clear_has_sequence_number() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void FileSingleHeader::clear_sequence_number() {
  sequence_number_ = 0u;
  clear_has_sequence_number();
}
inline ::google::protobuf::uint32 FileSingleHeader::sequence_number() const {
  return sequence_number_;
}
inline void FileSingleHeader::set_sequence_number(::google::protobuf::uint32 value) {
  set_has_sequence_number();
  sequence_number_ = value;
}

// required string meta_data = 5;
inline bool FileSingleHeader::has_meta_data() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void FileSingleHeader::set_has_meta_data() {
  _has_bits_[0] |= 0x00000010u;
}
inline void FileSingleHeader::clear_has_meta_data() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void FileSingleHeader::clear_meta_data() {
  if (meta_data_ != &::google::protobuf::internal::kEmptyString) {
    meta_data_->clear();
  }
  clear_has_meta_data();
}
inline const ::std::string& FileSingleHeader::meta_data() const {
  return *meta_data_;
}
inline void FileSingleHeader::set_meta_data(const ::std::string& value) {
  set_has_meta_data();
  if (meta_data_ == &::google::protobuf::internal::kEmptyString) {
    meta_data_ = new ::std::string;
  }
  meta_data_->assign(value);
}
inline void FileSingleHeader::set_meta_data(const char* value) {
  set_has_meta_data();
  if (meta_data_ == &::google::protobuf::internal::kEmptyString) {
    meta_data_ = new ::std::string;
  }
  meta_data_->assign(value);
}
inline void FileSingleHeader::set_meta_data(const char* value, size_t size) {
  set_has_meta_data();
  if (meta_data_ == &::google::protobuf::internal::kEmptyString) {
    meta_data_ = new ::std::string;
  }
  meta_data_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FileSingleHeader::mutable_meta_data() {
  set_has_meta_data();
  if (meta_data_ == &::google::protobuf::internal::kEmptyString) {
    meta_data_ = new ::std::string;
  }
  return meta_data_;
}
inline ::std::string* FileSingleHeader::release_meta_data() {
  clear_has_meta_data();
  if (meta_data_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = meta_data_;
    meta_data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required uint64 payload_size = 6 [default = 0];
inline bool FileSingleHeader::has_payload_size() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void FileSingleHeader::set_has_payload_size() {
  _has_bits_[0] |= 0x00000020u;
}
inline void FileSingleHeader::clear_has_payload_size() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void FileSingleHeader::clear_payload_size() {
  payload_size_ = GOOGLE_ULONGLONG(0);
  clear_has_payload_size();
}
inline ::google::protobuf::uint64 FileSingleHeader::payload_size() const {
  return payload_size_;
}
inline void FileSingleHeader::set_payload_size(::google::protobuf::uint64 value) {
  set_has_payload_size();
  payload_size_ = value;
}

// required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
inline bool FileSingleHeader::has_compression() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void FileSingleHeader::set_has_compression() {
  _has_bits_[0] |= 0x00000040u;
}
inline void FileSingleHeader::clear_has_compression() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void FileSingleHeader::clear_compression() {
  compression_ = 0;
  clear_has_compression();
}
inline ::QDLIB::FileSingleHeader_Compression FileSingleHeader::compression() const {
  return static_cast< ::QDLIB::FileSingleHeader_Compression >(compression_);
}
inline void FileSingleHeader::set_compression(::QDLIB::FileSingleHeader_Compression value) {
  GOOGLE_DCHECK(::QDLIB::FileSingleHeader_Compression_IsValid(value));
  set_has_compression();
  compression_ = value;
}

// optional bytes sub_header = 10;
inline bool FileSingleHeader::has_sub_header() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void FileSingleHeader::set_has_sub_header() {
  _has_bits_[0] |= 0x00000080u;
}
inline void FileSingleHeader::clear_has_sub_header() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void FileSingleHeader::clear_sub_header() {
  if (sub_header_ != &::google::protobuf::internal::kEmptyString) {
    sub_header_->clear();
  }
  clear_has_sub_header();
}
inline const ::std::string& FileSingleHeader::sub_header() const {
  return *sub_header_;
}
inline void FileSingleHeader::set_sub_header(const ::std::string& value) {
  set_has_sub_header();
  if (sub_header_ == &::google::protobuf::internal::kEmptyString) {
    sub_header_ = new ::std::string;
  }
  sub_header_->assign(value);
}
inline void FileSingleHeader::set_sub_header(const char* value) {
  set_has_sub_header();
  if (sub_header_ == &::google::protobuf::internal::kEmptyString) {
    sub_header_ = new ::std::string;
  }
  sub_header_->assign(value);
}
inline void FileSingleHeader::set_sub_header(const void* value, size_t size) {
  set_has_sub_header();
  if (sub_header_ == &::google::protobuf::internal::kEmptyString) {
    sub_header_ = new ::std::string;
  }
  sub_header_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FileSingleHeader::mutable_sub_header() {
  set_has_sub_header();
  if (sub_header_ == &::google::protobuf::internal::kEmptyString) {
    sub_header_ = new ::std::string;
  }
  return sub_header_;
}
inline ::std::string* FileSingleHeader::release_sub_header() {
  clear_has_sub_header();
  if (sub_header_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = sub_header_;
    sub_header_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// MultiStateIdentifier

// required int32 state = 1;
inline bool MultiStateIdentifier::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MultiStateIdentifier::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MultiStateIdentifier::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MultiStateIdentifier::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 MultiStateIdentifier::state() const {
  return state_;
}
inline void MultiStateIdentifier::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QDLIB

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::QDLIB::FileSingleHeader_Compression>() {
  return ::QDLIB::FileSingleHeader_Compression_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_FileSingleHeader_2eproto__INCLUDED