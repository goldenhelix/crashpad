// Copyright 2017 The Crashpad Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CRASHPAD_UTIL_LINUX_PTRACE_CLIENT_H_
#define CRASHPAD_UTIL_LINUX_PTRACE_CLIENT_H_

#include <sys/types.h>

#include "base/macros.h"
#include "util/linux/ptrace_connection.h"
#include "util/misc/address_types.h"
#include "util/misc/initialization_state_dcheck.h"

namespace crashpad {

//! \brief Implements a PtraceConnection over a socket.
//!
//! This class forms the client half of the connection and is typically used
//! when the current process does not have `ptrace` capabilities on the target
//! process. It should be created with a socket connected to a PtraceBroker.
class PtraceClient : public PtraceConnection {
 public:
  PtraceClient();
  ~PtraceClient();

  //! \brief Initializes this object.
  //!
  //! This method must be successfully called before any other method in this
  //! class.
  //!
  //! \param[in] sock A socket connected to a PtraceBroker. Does not take
  //!     ownership of the socket.
  //! \param[in] pid The process ID of the process to form a PtraceConnection
  //!     with.
  //! \return `true` on success. `false` on failure with a message logged.
  bool Initialize(int sock, pid_t pid);

  //! \brief Copies memory from the target process into a caller-provided buffer
  //!     in the current process.
  //!
  //! TODO(jperaza): In order for this to be usable, PtraceConnection will need
  //! to surface it, possibly by inheriting from ProcessMemory, or providing a
  //! method to return a ProcessMemory*.
  //!
  //! \param[in] address The address, in the target process' address space, of
  //!     the memory region to copy.
  //! \param[in] size The size, in bytes, of the memory region to copy.
  //!     \a buffer must be at least this size.
  //! \param[out] buffer The buffer into which the contents of the other
  //!     process' memory will be copied.
  //!
  //! \return `true` on success, with \a buffer filled appropriately. `false` on
  //!     failure, with a message logged.
  bool Read(VMAddress address, size_t size, void* buffer);

  // PtraceConnection:

  pid_t GetProcessID() override;
  bool Attach(pid_t tid) override;
  bool Is64Bit() override;
  bool GetThreadInfo(pid_t tid, ThreadInfo* info) override;

 private:
  int sock_;
  pid_t pid_;
  bool is_64_bit_;
  InitializationStateDcheck initialized_;

  DISALLOW_COPY_AND_ASSIGN(PtraceClient);
};

}  // namespace crashpad

#endif  // CRASHPAD_UTIL_LINUX_PTRACE_CLIENT_H_