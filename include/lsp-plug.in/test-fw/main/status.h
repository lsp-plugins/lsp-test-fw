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
        typedef int                 status_t;

        enum status_codes
        {
            STATUS_OK,                    //!< Successful status
            STATUS_UNKNOWN_ERR,           //!< Unknown error occurred
            STATUS_NO_MEM,                //!< Not enough memory
            STATUS_NO_DATA,               //!< No data at the input
            STATUS_BAD_ARGUMENTS,         //!< Invalid arguments
            STATUS_BAD_STATE,             //!< Bad state of the object/data structure
            STATUS_DUPLICATED,            //!< Duplicated value
            STATUS_PERMISSION_DENIED,     //!< Permission denied while accessing the resource
            STATUS_IO_ERROR,              //!< Generic input/output error
            STATUS_INVALID_VALUE,         //!< Invalid value passed as an argument
            STATUS_INSUFFICIENT,          //!< Insufficient arguments
            STATUS_KILLED,                //!< The task has been gracefully terminated
            STATUS_TIMED_OUT,             //!< The task has timed out
            STATUS_FAILED,                //!< The task has failed
            STATUS_NOT_DIRECTORY,         //!< The accessed object is not a directory
            STATUS_NOT_FOUND,             //!< The object was not found

            STATUS_TOTAL,
            STATUS_MAX = STATUS_TOTAL - 1,
            STATUS_SUCCESS = STATUS_OK    //!< STATUS_SUCCESS an alias to STATUS_OK
        };
    } /* namespace test */
} /* namespace lsp */

#endif /* INCLUDE_LSP_PLUG_IN_TEST_FW_MAIN_STATUS_H_ */
