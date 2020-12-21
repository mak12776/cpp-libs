#pragma once

namespace scl
{
	namespace winapi
	{
		static inline int safe_read(int fd, void *pntr, unsigned int size)
		{
			int total = _read(fd, pntr, size);

			if (total != size)
			{
				scl::err::set(scl::err::READ);
				scl::err::push(__FILE__, __LINE__, __FUNCTION__);
			}

			return total;
		}

		static inline int safe_write(int fd, void *pntr, unsigned int size)
		{
			int total = _write(fd, pntr, size);

			if (total != size)
			{
				scl::err::set(scl::err::WRITE);
				scl::err::push(__FILE__, __LINE__, __FUNCTION__);
			}

			return total;
		}

		void safe_set_priority_class(HANDLE process_handle, DWORD Priority)
		{
			using namespace scl;

			if (!SetPriorityClass(process_handle, Priority))
			{
				scl::err::set(err::WIN_ERROR);
				scl::err::push(__FILE__, __LINE__, __FUNCTION__);
			}
		}
	}
}