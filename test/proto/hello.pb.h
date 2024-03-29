// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/hello.proto

#ifndef PROTOBUF_proto_2fhello_2eproto__INCLUDED
#define PROTOBUF_proto_2fhello_2eproto__INCLUDED

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
#include <google/protobuf/service.h>
// @@protoc_insertion_point(includes)

namespace trpc {
namespace hellotest {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_proto_2fhello_2eproto();
void protobuf_AssignDesc_proto_2fhello_2eproto();
void protobuf_ShutdownFile_proto_2fhello_2eproto();

class HelloRequest;
class HelloResponse;

// ===================================================================

class HelloRequest : public ::google::protobuf::Message {
 public:
  HelloRequest();
  virtual ~HelloRequest();
  
  HelloRequest(const HelloRequest& from);
  
  inline HelloRequest& operator=(const HelloRequest& from) {
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
  static const HelloRequest& default_instance();
  
  void Swap(HelloRequest* other);
  
  // implements Message ----------------------------------------------
  
  HelloRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HelloRequest& from);
  void MergeFrom(const HelloRequest& from);
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
  
  // optional uint32 hello_id = 1;
  inline bool has_hello_id() const;
  inline void clear_hello_id();
  static const int kHelloIdFieldNumber = 1;
  inline ::google::protobuf::uint32 hello_id() const;
  inline void set_hello_id(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:trpc.hellotest.HelloRequest)
 private:
  inline void set_has_hello_id();
  inline void clear_has_hello_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 hello_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_proto_2fhello_2eproto();
  friend void protobuf_AssignDesc_proto_2fhello_2eproto();
  friend void protobuf_ShutdownFile_proto_2fhello_2eproto();
  
  void InitAsDefaultInstance();
  static HelloRequest* default_instance_;
};
// -------------------------------------------------------------------

class HelloResponse : public ::google::protobuf::Message {
 public:
  HelloResponse();
  virtual ~HelloResponse();
  
  HelloResponse(const HelloResponse& from);
  
  inline HelloResponse& operator=(const HelloResponse& from) {
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
  static const HelloResponse& default_instance();
  
  void Swap(HelloResponse* other);
  
  // implements Message ----------------------------------------------
  
  HelloResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HelloResponse& from);
  void MergeFrom(const HelloResponse& from);
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
  
  // optional uint32 hello_id = 1;
  inline bool has_hello_id() const;
  inline void clear_hello_id();
  static const int kHelloIdFieldNumber = 1;
  inline ::google::protobuf::uint32 hello_id() const;
  inline void set_hello_id(::google::protobuf::uint32 value);
  
  // optional string hello_msg = 2;
  inline bool has_hello_msg() const;
  inline void clear_hello_msg();
  static const int kHelloMsgFieldNumber = 2;
  inline const ::std::string& hello_msg() const;
  inline void set_hello_msg(const ::std::string& value);
  inline void set_hello_msg(const char* value);
  inline void set_hello_msg(const char* value, size_t size);
  inline ::std::string* mutable_hello_msg();
  inline ::std::string* release_hello_msg();
  
  // @@protoc_insertion_point(class_scope:trpc.hellotest.HelloResponse)
 private:
  inline void set_has_hello_id();
  inline void clear_has_hello_id();
  inline void set_has_hello_msg();
  inline void clear_has_hello_msg();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* hello_msg_;
  ::google::protobuf::uint32 hello_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_proto_2fhello_2eproto();
  friend void protobuf_AssignDesc_proto_2fhello_2eproto();
  friend void protobuf_ShutdownFile_proto_2fhello_2eproto();
  
  void InitAsDefaultInstance();
  static HelloResponse* default_instance_;
};
// ===================================================================

class HelloService_Stub;

class HelloService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline HelloService() {};
 public:
  virtual ~HelloService();
  
  typedef HelloService_Stub Stub;
  
  static const ::google::protobuf::ServiceDescriptor* descriptor();
  
  virtual void Hello(::google::protobuf::RpcController* controller,
                       const ::trpc::hellotest::HelloRequest* request,
                       ::trpc::hellotest::HelloResponse* response,
                       ::google::protobuf::Closure* done);
  
  // implements Service ----------------------------------------------
  
  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(HelloService);
};

class HelloService_Stub : public HelloService {
 public:
  HelloService_Stub(::google::protobuf::RpcChannel* channel);
  HelloService_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~HelloService_Stub();
  
  inline ::google::protobuf::RpcChannel* channel() { return channel_; }
  
  // implements HelloService ------------------------------------------
  
  void Hello(::google::protobuf::RpcController* controller,
                       const ::trpc::hellotest::HelloRequest* request,
                       ::trpc::hellotest::HelloResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(HelloService_Stub);
};


// ===================================================================


// ===================================================================

// HelloRequest

// optional uint32 hello_id = 1;
inline bool HelloRequest::has_hello_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HelloRequest::set_has_hello_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HelloRequest::clear_has_hello_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HelloRequest::clear_hello_id() {
  hello_id_ = 0u;
  clear_has_hello_id();
}
inline ::google::protobuf::uint32 HelloRequest::hello_id() const {
  return hello_id_;
}
inline void HelloRequest::set_hello_id(::google::protobuf::uint32 value) {
  set_has_hello_id();
  hello_id_ = value;
}

// -------------------------------------------------------------------

// HelloResponse

// optional uint32 hello_id = 1;
inline bool HelloResponse::has_hello_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HelloResponse::set_has_hello_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HelloResponse::clear_has_hello_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HelloResponse::clear_hello_id() {
  hello_id_ = 0u;
  clear_has_hello_id();
}
inline ::google::protobuf::uint32 HelloResponse::hello_id() const {
  return hello_id_;
}
inline void HelloResponse::set_hello_id(::google::protobuf::uint32 value) {
  set_has_hello_id();
  hello_id_ = value;
}

// optional string hello_msg = 2;
inline bool HelloResponse::has_hello_msg() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void HelloResponse::set_has_hello_msg() {
  _has_bits_[0] |= 0x00000002u;
}
inline void HelloResponse::clear_has_hello_msg() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void HelloResponse::clear_hello_msg() {
  if (hello_msg_ != &::google::protobuf::internal::kEmptyString) {
    hello_msg_->clear();
  }
  clear_has_hello_msg();
}
inline const ::std::string& HelloResponse::hello_msg() const {
  return *hello_msg_;
}
inline void HelloResponse::set_hello_msg(const ::std::string& value) {
  set_has_hello_msg();
  if (hello_msg_ == &::google::protobuf::internal::kEmptyString) {
    hello_msg_ = new ::std::string;
  }
  hello_msg_->assign(value);
}
inline void HelloResponse::set_hello_msg(const char* value) {
  set_has_hello_msg();
  if (hello_msg_ == &::google::protobuf::internal::kEmptyString) {
    hello_msg_ = new ::std::string;
  }
  hello_msg_->assign(value);
}
inline void HelloResponse::set_hello_msg(const char* value, size_t size) {
  set_has_hello_msg();
  if (hello_msg_ == &::google::protobuf::internal::kEmptyString) {
    hello_msg_ = new ::std::string;
  }
  hello_msg_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HelloResponse::mutable_hello_msg() {
  set_has_hello_msg();
  if (hello_msg_ == &::google::protobuf::internal::kEmptyString) {
    hello_msg_ = new ::std::string;
  }
  return hello_msg_;
}
inline ::std::string* HelloResponse::release_hello_msg() {
  clear_has_hello_msg();
  if (hello_msg_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = hello_msg_;
    hello_msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace hellotest
}  // namespace trpc

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_proto_2fhello_2eproto__INCLUDED
