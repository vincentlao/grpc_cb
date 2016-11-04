// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef GRPC_CB_CLIENT_CLIENT_SYNC_READER_WRITER_H
#define GRPC_CB_CLIENT_CLIENT_SYNC_READER_WRITER_H

#include <grpc_cb/impl/client/client_sync_reader_writer_impl.h>  // for ClientSyncReaderWriterImpl<>

namespace grpc_cb {

// Copyable.
template <class Request, class Response>
class ClientSyncReaderWriter GRPC_FINAL {
 public:
  ClientSyncReaderWriter(const ChannelSptr& channel, const std::string& method)
                            // XXX const CompletionQueueSptr& cq_sptr)
      // Todo: same as ClientReader?
      : impl_sptr_(new Impl(channel, method)) {
    assert(channel);
  }

 public:
  // Write is always asynchronous.
  bool Write(const Request& request) const {
    return impl_sptr_->Write(request);
  }

  // Optional. Writes are auto done in dtr().
  // Redundant calls are ignored.
  void WritesDone() {
    impl_sptr_->WritesDone();
  }

  bool BlockingReadOne(Response* response) const {
    return impl_sptr_->BlockingReadOne(response);
  }

  Status BlockingRecvStatus() const {
    return impl_sptr_->BlockingRecvStatus();
  }

  using ReadCallback = std::function<void(const Response&)>;
  void AsyncReadEach(
      const ReadCallback& on_read,
      const StatusCallback& on_status = StatusCallback()) const {
    impl_sptr_->AsyncReadEach(on_read, on_status);
  }

 private:
  using Impl = ClientSyncReaderWriterImpl<Request, Response>;
  std::shared_ptr<Impl> impl_sptr_;
};  // class ClientSyncReaderWriter<>

// Todo: BlockingGetInitMd();

// Todo: same as ClientReader?

}  // namespace grpc_cb

#endif  // GRPC_CB_CLIENT_CLIENT_SYNC_READER_WRITER_H
