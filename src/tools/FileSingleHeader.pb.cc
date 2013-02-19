// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "FileSingleHeader.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace QDLIB {

namespace {

const ::google::protobuf::Descriptor* FileSingleHeader_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  FileSingleHeader_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* FileSingleHeader_Compression_descriptor_ = NULL;
const ::google::protobuf::Descriptor* MultiStateIdentifier_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MultiStateIdentifier_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_FileSingleHeader_2eproto() {
  protobuf_AddDesc_FileSingleHeader_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "FileSingleHeader.proto");
  GOOGLE_CHECK(file != NULL);
  FileSingleHeader_descriptor_ = file->message_type(0);
  static const int FileSingleHeader_offsets_[8] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, more_files_follow_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, class__),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, sequence_number_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, meta_data_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, payload_size_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, compression_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, sub_header_),
  };
  FileSingleHeader_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      FileSingleHeader_descriptor_,
      FileSingleHeader::default_instance_,
      FileSingleHeader_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FileSingleHeader, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(FileSingleHeader));
  FileSingleHeader_Compression_descriptor_ = FileSingleHeader_descriptor_->enum_type(0);
  MultiStateIdentifier_descriptor_ = file->message_type(1);
  static const int MultiStateIdentifier_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MultiStateIdentifier, state_),
  };
  MultiStateIdentifier_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MultiStateIdentifier_descriptor_,
      MultiStateIdentifier::default_instance_,
      MultiStateIdentifier_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MultiStateIdentifier, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MultiStateIdentifier, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MultiStateIdentifier));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_FileSingleHeader_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    FileSingleHeader_descriptor_, &FileSingleHeader::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MultiStateIdentifier_descriptor_, &MultiStateIdentifier::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_FileSingleHeader_2eproto() {
  delete FileSingleHeader::default_instance_;
  delete FileSingleHeader_reflection_;
  delete MultiStateIdentifier::default_instance_;
  delete MultiStateIdentifier_reflection_;
}

void protobuf_AddDesc_FileSingleHeader_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026FileSingleHeader.proto\022\005QDLIB\"\247\002\n\020File"
    "SingleHeader\022 \n\021more_files_follow\030\001 \002(\010:"
    "\005false\022\r\n\005class\030\002 \002(\t\022\014\n\004name\030\003 \001(\t\022\027\n\017s"
    "equence_number\030\004 \001(\r\022\021\n\tmeta_data\030\005 \002(\t\022"
    "\027\n\014payload_size\030\006 \002(\004:\0010\022F\n\013compression\030"
    "\007 \002(\0162#.QDLIB.FileSingleHeader.Compressi"
    "on:\014UNCOMPRESSED\022\022\n\nsub_header\030\n \001(\014\"3\n\013"
    "Compression\022\020\n\014UNCOMPRESSED\020\000\022\010\n\004ZLIB\020\001\022"
    "\010\n\004BZIP\020\002\"%\n\024MultiStateIdentifier\022\r\n\005sta"
    "te\030\001 \002(\005", 368);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "FileSingleHeader.proto", &protobuf_RegisterTypes);
  FileSingleHeader::default_instance_ = new FileSingleHeader();
  MultiStateIdentifier::default_instance_ = new MultiStateIdentifier();
  FileSingleHeader::default_instance_->InitAsDefaultInstance();
  MultiStateIdentifier::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_FileSingleHeader_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_FileSingleHeader_2eproto {
  StaticDescriptorInitializer_FileSingleHeader_2eproto() {
    protobuf_AddDesc_FileSingleHeader_2eproto();
  }
} static_descriptor_initializer_FileSingleHeader_2eproto_;


// ===================================================================

const ::google::protobuf::EnumDescriptor* FileSingleHeader_Compression_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FileSingleHeader_Compression_descriptor_;
}
bool FileSingleHeader_Compression_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const FileSingleHeader_Compression FileSingleHeader::UNCOMPRESSED;
const FileSingleHeader_Compression FileSingleHeader::ZLIB;
const FileSingleHeader_Compression FileSingleHeader::BZIP;
const FileSingleHeader_Compression FileSingleHeader::Compression_MIN;
const FileSingleHeader_Compression FileSingleHeader::Compression_MAX;
const int FileSingleHeader::Compression_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int FileSingleHeader::kMoreFilesFollowFieldNumber;
const int FileSingleHeader::kClassFieldNumber;
const int FileSingleHeader::kNameFieldNumber;
const int FileSingleHeader::kSequenceNumberFieldNumber;
const int FileSingleHeader::kMetaDataFieldNumber;
const int FileSingleHeader::kPayloadSizeFieldNumber;
const int FileSingleHeader::kCompressionFieldNumber;
const int FileSingleHeader::kSubHeaderFieldNumber;
#endif  // !_MSC_VER

FileSingleHeader::FileSingleHeader()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void FileSingleHeader::InitAsDefaultInstance() {
}

FileSingleHeader::FileSingleHeader(const FileSingleHeader& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void FileSingleHeader::SharedCtor() {
  _cached_size_ = 0;
  more_files_follow_ = false;
  class__ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  sequence_number_ = 0u;
  meta_data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  payload_size_ = GOOGLE_ULONGLONG(0);
  compression_ = 0;
  sub_header_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FileSingleHeader::~FileSingleHeader() {
  SharedDtor();
}

void FileSingleHeader::SharedDtor() {
  if (class__ != &::google::protobuf::internal::kEmptyString) {
    delete class__;
  }
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (meta_data_ != &::google::protobuf::internal::kEmptyString) {
    delete meta_data_;
  }
  if (sub_header_ != &::google::protobuf::internal::kEmptyString) {
    delete sub_header_;
  }
  if (this != default_instance_) {
  }
}

void FileSingleHeader::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FileSingleHeader::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FileSingleHeader_descriptor_;
}

const FileSingleHeader& FileSingleHeader::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_FileSingleHeader_2eproto();  return *default_instance_;
}

FileSingleHeader* FileSingleHeader::default_instance_ = NULL;

FileSingleHeader* FileSingleHeader::New() const {
  return new FileSingleHeader;
}

void FileSingleHeader::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    more_files_follow_ = false;
    if (has_class_()) {
      if (class__ != &::google::protobuf::internal::kEmptyString) {
        class__->clear();
      }
    }
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::kEmptyString) {
        name_->clear();
      }
    }
    sequence_number_ = 0u;
    if (has_meta_data()) {
      if (meta_data_ != &::google::protobuf::internal::kEmptyString) {
        meta_data_->clear();
      }
    }
    payload_size_ = GOOGLE_ULONGLONG(0);
    compression_ = 0;
    if (has_sub_header()) {
      if (sub_header_ != &::google::protobuf::internal::kEmptyString) {
        sub_header_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool FileSingleHeader::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bool more_files_follow = 1 [default = false];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &more_files_follow_)));
          set_has_more_files_follow();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_class;
        break;
      }
      
      // required string class = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_class:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_class_()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->class_().data(), this->class_().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_name;
        break;
      }
      
      // optional string name = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_sequence_number;
        break;
      }
      
      // optional uint32 sequence_number = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_sequence_number:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &sequence_number_)));
          set_has_sequence_number();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_meta_data;
        break;
      }
      
      // required string meta_data = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_meta_data:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_meta_data()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->meta_data().data(), this->meta_data().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_payload_size;
        break;
      }
      
      // required uint64 payload_size = 6 [default = 0];
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_payload_size:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &payload_size_)));
          set_has_payload_size();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_compression;
        break;
      }
      
      // required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_compression:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::QDLIB::FileSingleHeader_Compression_IsValid(value)) {
            set_compression(static_cast< ::QDLIB::FileSingleHeader_Compression >(value));
          } else {
            mutable_unknown_fields()->AddVarint(7, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(82)) goto parse_sub_header;
        break;
      }
      
      // optional bytes sub_header = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_sub_header:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_sub_header()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void FileSingleHeader::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required bool more_files_follow = 1 [default = false];
  if (has_more_files_follow()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->more_files_follow(), output);
  }
  
  // required string class = 2;
  if (has_class_()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->class_().data(), this->class_().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->class_(), output);
  }
  
  // optional string name = 3;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->name(), output);
  }
  
  // optional uint32 sequence_number = 4;
  if (has_sequence_number()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->sequence_number(), output);
  }
  
  // required string meta_data = 5;
  if (has_meta_data()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->meta_data().data(), this->meta_data().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->meta_data(), output);
  }
  
  // required uint64 payload_size = 6 [default = 0];
  if (has_payload_size()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(6, this->payload_size(), output);
  }
  
  // required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
  if (has_compression()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      7, this->compression(), output);
  }
  
  // optional bytes sub_header = 10;
  if (has_sub_header()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      10, this->sub_header(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* FileSingleHeader::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required bool more_files_follow = 1 [default = false];
  if (has_more_files_follow()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->more_files_follow(), target);
  }
  
  // required string class = 2;
  if (has_class_()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->class_().data(), this->class_().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->class_(), target);
  }
  
  // optional string name = 3;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->name(), target);
  }
  
  // optional uint32 sequence_number = 4;
  if (has_sequence_number()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->sequence_number(), target);
  }
  
  // required string meta_data = 5;
  if (has_meta_data()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->meta_data().data(), this->meta_data().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->meta_data(), target);
  }
  
  // required uint64 payload_size = 6 [default = 0];
  if (has_payload_size()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(6, this->payload_size(), target);
  }
  
  // required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
  if (has_compression()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      7, this->compression(), target);
  }
  
  // optional bytes sub_header = 10;
  if (has_sub_header()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        10, this->sub_header(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int FileSingleHeader::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bool more_files_follow = 1 [default = false];
    if (has_more_files_follow()) {
      total_size += 1 + 1;
    }
    
    // required string class = 2;
    if (has_class_()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->class_());
    }
    
    // optional string name = 3;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }
    
    // optional uint32 sequence_number = 4;
    if (has_sequence_number()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->sequence_number());
    }
    
    // required string meta_data = 5;
    if (has_meta_data()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->meta_data());
    }
    
    // required uint64 payload_size = 6 [default = 0];
    if (has_payload_size()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->payload_size());
    }
    
    // required .QDLIB.FileSingleHeader.Compression compression = 7 [default = UNCOMPRESSED];
    if (has_compression()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->compression());
    }
    
    // optional bytes sub_header = 10;
    if (has_sub_header()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->sub_header());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FileSingleHeader::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const FileSingleHeader* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const FileSingleHeader*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void FileSingleHeader::MergeFrom(const FileSingleHeader& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_more_files_follow()) {
      set_more_files_follow(from.more_files_follow());
    }
    if (from.has_class_()) {
      set_class_(from.class_());
    }
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_sequence_number()) {
      set_sequence_number(from.sequence_number());
    }
    if (from.has_meta_data()) {
      set_meta_data(from.meta_data());
    }
    if (from.has_payload_size()) {
      set_payload_size(from.payload_size());
    }
    if (from.has_compression()) {
      set_compression(from.compression());
    }
    if (from.has_sub_header()) {
      set_sub_header(from.sub_header());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void FileSingleHeader::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FileSingleHeader::CopyFrom(const FileSingleHeader& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FileSingleHeader::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000073) != 0x00000073) return false;
  
  return true;
}

void FileSingleHeader::Swap(FileSingleHeader* other) {
  if (other != this) {
    std::swap(more_files_follow_, other->more_files_follow_);
    std::swap(class__, other->class__);
    std::swap(name_, other->name_);
    std::swap(sequence_number_, other->sequence_number_);
    std::swap(meta_data_, other->meta_data_);
    std::swap(payload_size_, other->payload_size_);
    std::swap(compression_, other->compression_);
    std::swap(sub_header_, other->sub_header_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata FileSingleHeader::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = FileSingleHeader_descriptor_;
  metadata.reflection = FileSingleHeader_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int MultiStateIdentifier::kStateFieldNumber;
#endif  // !_MSC_VER

MultiStateIdentifier::MultiStateIdentifier()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void MultiStateIdentifier::InitAsDefaultInstance() {
}

MultiStateIdentifier::MultiStateIdentifier(const MultiStateIdentifier& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void MultiStateIdentifier::SharedCtor() {
  _cached_size_ = 0;
  state_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MultiStateIdentifier::~MultiStateIdentifier() {
  SharedDtor();
}

void MultiStateIdentifier::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MultiStateIdentifier::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MultiStateIdentifier::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MultiStateIdentifier_descriptor_;
}

const MultiStateIdentifier& MultiStateIdentifier::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_FileSingleHeader_2eproto();  return *default_instance_;
}

MultiStateIdentifier* MultiStateIdentifier::default_instance_ = NULL;

MultiStateIdentifier* MultiStateIdentifier::New() const {
  return new MultiStateIdentifier;
}

void MultiStateIdentifier::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    state_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MultiStateIdentifier::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 state = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &state_)));
          set_has_state();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void MultiStateIdentifier::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 state = 1;
  if (has_state()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->state(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MultiStateIdentifier::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 state = 1;
  if (has_state()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->state(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MultiStateIdentifier::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 state = 1;
    if (has_state()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->state());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MultiStateIdentifier::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MultiStateIdentifier* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MultiStateIdentifier*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MultiStateIdentifier::MergeFrom(const MultiStateIdentifier& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_state()) {
      set_state(from.state());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MultiStateIdentifier::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MultiStateIdentifier::CopyFrom(const MultiStateIdentifier& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MultiStateIdentifier::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void MultiStateIdentifier::Swap(MultiStateIdentifier* other) {
  if (other != this) {
    std::swap(state_, other->state_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MultiStateIdentifier::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MultiStateIdentifier_descriptor_;
  metadata.reflection = MultiStateIdentifier_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QDLIB

// @@protoc_insertion_point(global_scope)