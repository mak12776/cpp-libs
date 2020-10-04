#pragma once

#include <stdint.h>
#include <cstdio>
#include <stdlib.h>
#include <stdarg.h>

namespace scl
{
	namespace cleaner
	{
		enum class job_type : uint8_t
		{
			FCLOSE, FREE, DELETE,
		};

		struct job
		{
			job_type type;
			union
			{
				FILE *file;
				void *pntr;
			};
		};

		constexpr size_t local_jobs_size = 4096;


		template <size_t array_size>
		struct cleaner_t
		{
			size_t index = 0;
			job jobs[array_size];

			inline void add_fclose(FILE *file)
			{
				if (index != array_size)
				{
					jobs[index].type = job_type::FCLOSE;
					jobs[index].file = file;
					index += 1;
				}
			}

			inline void add_free(void *pntr)
			{
				if (index != array_size)
				{
					jobs[index].type = job_type::FREE;
					jobs[index].pntr = pntr;
					index += 1;
				}
			}

			inline void add_delete(void *pntr)
			{
				if (index != array_size)
				{
					jobs[index].type = job_type::DELETE;
					jobs[index].pntr = pntr;
					index += 1;
				}
			}

			inline void finish()
			{
				for (size_t index = 0; index < index; index += 1)
				{
					switch (jobs[index].type)
					{
					case job_type::FCLOSE:
						fclose(jobs[index].file);
						break;

					case job_type::FREE:
						free(jobs[index].pntr);
						break;

					case job_type::DELETE:
						delete jobs[index].pntr;
						break;
					}
				}
			}
		};

		cleaner_t<local_jobs_size> local_storage;

		void add_fclose(FILE *file) { local_storage.add_fclose(file); }
		void add_free(void *pntr) { local_storage.add_free(pntr); }
		void add_delete(void *pntr) { local_storage.add_delete(pntr); }
		void finish() { local_storage.finish(); }
	}
}
