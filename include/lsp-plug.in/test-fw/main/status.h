/*
 * Copyright (C) 2022 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2022 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 30 июн. 2022 г.
 *
 * lsp-test-fw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-test-fw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-test-fw. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_TEST_FW_MAIN_STATUS_H_
#define LSP_PLUG_IN_TEST_FW_MAIN_STATUS_H_

namespace lsp
{
    namespace test
    {
        // Status codes
        typedef int                 test_status_t;

        enum LSP_TEST_FW_codes
        {
            LSP_TEST_FW_OK,                     //!< Successful status
            LSP_TEST_FW_UNKNOWN_ERR,            //!< Unknown error occurred
            LSP_TEST_FW_NO_MEM,                 //!< Not enough memory
            LSP_TEST_FW_NO_DATA,                //!< No data at the input
            LSP_TEST_FW_BAD_ARGUMENTS,          //!< Invalid arguments
            LSP_TEST_FW_BAD_STATE,              //!< Bad state of the object/data structure
            LSP_TEST_FW_DUPLICATED,             //!< Duplicated value
            LSP_TEST_FW_PERMISSION_DENIED,      //!< Permission denied while accessing the resource
            LSP_TEST_FW_IO_ERROR,               //!< Generic input/output error
            LSP_TEST_FW_INVALID_VALUE,          //!< Invalid value passed as an argument
            LSP_TEST_FW_INSUFFICIENT,           //!< Insufficient arguments
            LSP_TEST_FW_KILLED,                 //!< The task has been gracefully terminated
            LSP_TEST_FW_TIMED_OUT,              //!< The task has timed out
            LSP_TEST_FW_FAILED,                 //!< The task has failed
            LSP_TEST_FW_NOT_DIRECTORY,          //!< The accessed object is not a directory
            LSP_TEST_FW_NOT_FOUND,              //!< The object was not found

            LSP_TEST_FW_TOTAL,
            LSP_TEST_FW_MAX         = LSP_TEST_FW_TOTAL - 1,
            LSP_TEST_FW_SUCCESS     = LSP_TEST_FW_OK    //!< Alias to LSP_TEST_FW_OK
        };
    } /* namespace test */
} /* namespace lsp */

#endif /* LSP_PLUG_IN_TEST_FW_MAIN_STATUS_H_ */
