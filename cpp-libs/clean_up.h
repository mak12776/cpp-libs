#pragma once

#include <stdint.h>
#include <cstdio>
#include <stdlib.h>
#include <stdarg.h>

namespace scl
{
	namespace clean_up
	{
		enum class job_type : uint8_t
		{
			FILE_CLOSE, MEMORY_FREE, MEMORY_DELETE,
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

		template <size_t array_size>
		struct storage
		{
			job jobs_array[array_size];
			size_t array_index;

			inline void add_fclose(FILE *file)
			{
				if (array_index != array_size)
				{
					jobs_array[array_index].type = job_type::FILE_CLOSE;
					jobs_array[array_index].file = file;
					array_index += 1;
				}
			}

			inline void add_free(void *pntr)
			{
				if (array_index != array_size)
				{
					jobs_array[array_index].type = job_type::MEMORY_FREE;
					jobs_array[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void add_delete(void *pntr)
			{
				if (array_index != array_size)
				{
					jobs_array[array_index].type = job_type::MEMORY_DELETE;
					jobs_array[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void finish()
			{
				for (size_t index = 0; index < array_index; index += 1)
				{
					switch (jobs_array[index].type)
					{
					case job_type::FILE_CLOSE:
						fclose(jobs_array[index].file);
						break;

					case job_type::MEMORY_FREE:
						free(jobs_array[index].pntr);
						break;

					case job_type::MEMORY_DELETE:
						delete jobs_array[index].pntr;
						break;
					}
				}
			}
		};

		constexpr size_t jobs_max = 1024;
		size_t jobs_index = 0;
		job jobs[jobs_max];


		void add_fclose(FILE *file)
		{
			if (jobs_index != jobs_max)
			{
				jobs[jobs_index].type = job_type::FILE_CLOSE;
				jobs[jobs_index].file = file;
				jobs_index += 1;
			}
		}

		void add_free(void *pntr)
		{
			if (jobs_index != jobs_max)
			{
				jobs[jobs_index].type = job_type::MEMORY_FREE;
				jobs[jobs_index].pntr = pntr;
				jobs_index += 1;
			}
		}

		void add_delete(void *pntr)
		{
			if (jobs_index != jobs_max)
			{
				jobs[jobs_index].type = job_type::MEMORY_DELETE;
				jobs[jobs_index].pntr = pntr;
				jobs_index += 1;
			}
		}

		void finish()
		{
			for (size_t index = 0; index < jobs_index; index += 1)
			{
				switch (jobs[index].type)
				{
				case job_type::FILE_CLOSE:
					fclose(jobs[index].file);
					break;

				case job_type::MEMORY_FREE:
					free(jobs[index].pntr);
					break;

				case job_type::MEMORY_DELETE:
					delete jobs[index].pntr;
					break;
				}
			}
		}
	}
}
