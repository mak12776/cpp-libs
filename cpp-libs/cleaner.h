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

		template <size_t size>
		struct cleaner_t
		{
			size_t index = 0;
			job jobs[size];

			inline void add_fclose(FILE *file)
			{
				if (index != size)
				{
					jobs[index].type = job_type::FCLOSE;
					jobs[index].file = file;
					index += 1;
				}
			}

			inline void add_free(void *pntr)
			{
				if (index != size)
				{
					jobs[index].type = job_type::FREE;
					jobs[index].pntr = pntr;
					index += 1;
				}
			}

			inline void add_delete(void *pntr)
			{
				if (index != size)
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

		constexpr size_t default_jobs_size = 4096;
		cleaner_t<default_jobs_size> default_cleaner;

		void add_fclose(FILE *file) { default_cleaner.add_fclose(file); }
		void add_free(void *pntr) { default_cleaner.add_free(pntr); }
		void add_delete(void *pntr) { default_cleaner.add_delete(pntr); }
		void finish() { default_cleaner.finish(); }
	}
}
