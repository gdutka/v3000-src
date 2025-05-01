# *****************************************************************************
# *
# * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
# *
# ******************************************************************************

# ******************************************************************************
# Command Strings
# ******************************************************************************

# ******************************************************************************
# Strings for *.c Shell Strings file
# ******************************************************************************
include_files = """
#include "AutoConfigOptions.h"
"""

start_variable_array = """
ACT_OPTION  TestVariableSet[] = {"""

end_variable_array = """
};
"""

start_variable_item = """
  {{
    L"{var_name:s}",
    L"{display_name:s}",
    L"{apcb_name:s}",
    L"{pcd_name:s}",
    {uid:s},
    {value:d},
    {nv_type:s},
    {var_count:d},
    Opt{var_name:s}Values"""

end_variable_item = """
  },"""

start_value_set = """
ACT_VALUE Opt{var_name:s}Values[] = {{"""

end_value_set = """
};
"""

end_value_file = """
UINTN TestSetSize = sizeof(TestVariableSet) / sizeof(*TestVariableSet);
"""

variable_value_pair = """
  VALUE_PAIRS({value:s}, L"{option_string:s}"),"""

uid_name = """ACT_CFG_UID_{var_name:s}"""



# ******************************************************************************
# Strings for *.h file
# ******************************************************************************
amd_header = """/******************************************************************************
 *   Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************/
"""

c_horiz_line = """
/*****************************************************************************/
"""

define_header = """
#ifndef {guard:s}_
#define {guard:s}_
"""

end_define_header = """
#endif //  {guard:s}_
"""

option_comment = """
/**
 * @brief   {pretty_name: <40s}         0x{uid:0>8X}
 * @details"""

option_help = """
 *    {help_line:s}"""

option_data_type = """
 *    Data Type: {data_type:s}
"""

option_category = """ *    Category: {format_section:s}"""

category_string_part = """ {format_section:s} > """

option_end_comment = """
 **/"""

define_option = """
#define    ACT_CFG_UID_{var_name: <50s}        0x{uid:0>8X}
"""

define_prefix = "ACT_CFG_UID_"

# ******************************************************************************
# Strings for Console Pretty Printing
# ******************************************************************************
console_csv_option = """{variable:s},{pretty_name:s},{uid:s},{in_type:s},{data_size:s},{default_value:d},{current_value:s},{pcd_name:s},{apcb_name:s},{help_text:s}
"""

console_option_display = """
Name:              {pretty_name:s}
Variable Name:     {variable:s}
Id:                {uid:s}
Input Type:        {in_type:s}
Data Size:         {data_size:s}
Default Value:     {default_value:d}
Current Value:     {current_value:s}
Pcd:               {pcd_name:s}
Apcb:              {apcb_name:s}
Description:
{help_text:s}
"""
