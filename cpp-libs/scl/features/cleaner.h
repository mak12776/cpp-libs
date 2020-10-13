#pragma once

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

		template <size_t _size>
		struct cleaner_t
		{
			size_t array_index = 0;
			job jobs[_size];

			constexpr size_t array_size() { return _size; }

			inline void add_fclose(FILE *file)
			{
				if (array_index != _size)
				{
					jobs[array_index].type = job_type::FCLOSE;
					jobs[array_index].file = file;
					array_index += 1;
				}
			}

			inline void add_free(void *pntr)
			{
				if (array_index != _size)
				{
					jobs[array_index].type = job_type::FREE;
					jobs[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void add_delete(void *pntr)
			{
				if (array_index != _size)
				{
					jobs[array_index].type = job_type::DELETE;
					jobs[array_index].pntr = pntr;
					array_index += 1;
				}
			}

			inline void finish()
			{
				for (size_t index = 0; index < array_index; index += 1)
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

		constexpr size_t default_array_size = 4096;
		
		// default cleaner_t
		typedef cleaner_t<default_array_size> default_cleaner_t;
		default_cleaner_t default_cleaner;

		void add_fclose(FILE *file) { default_cleaner.add_fclose(file); }
		void add_free(void *pntr) { default_cleaner.add_free(pntr); }
		void add_delete(void *pntr) { default_cleaner.add_delete(pntr); }
		void finish() { default_cleaner.finish(); }
	}
}
