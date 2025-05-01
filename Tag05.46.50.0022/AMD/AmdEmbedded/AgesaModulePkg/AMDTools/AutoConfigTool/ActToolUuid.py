"""
*******************************************************************************
 Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.

*******************************************************************************
"""
import mmh3

HASH_MIN = 0
UUID_LEN_MASK = 0xFFFFFFFF
SEED_VALUE = 0x1111

class AmdUuid:
    """!
        This class is for generating a new, unique uuid that doesn't conflict
        with any existing uuids.

        Properties:
            UuidValue       - A Generated 32 bit UUID
            SeedValue       - A starting value for hashing.  We use this when there
                              are UID conflicts.
            Name            - Name of the variable the guid is being generated for.
            HashMin         - Minimum value for the hash. Default 0x0
            HashMax         - Maximum value for the hash. Default 0xFFFFFFFF
    """
    def __init__(self, name, hash_min = HASH_MIN, hash_max = UUID_LEN_MASK, seed_value = SEED_VALUE):
        """!
            This initialization routine for AmdUuid generates populates initialization
            data for the UUID object and generates a new UUID.
        """
        self.Name = name
        self.HashMin = hash_min
        self.HashMax = hash_max
        self.SeedValue = seed_value
        self.UuidValue = self.genHashVal()

    def genHashVal(self):
        """!
            Generates and returns a 32-bit truncated integer uuid using the murmur
            hash library.

            The Seed Value is incremented so if there is a UID conflict we can generate a
            unique number.
        """
        hashVal = int(mmh3.hash(self.Name, self.SeedValue)) & self.HashMax
        if (self.HashMin != 0 or
            self.HashMax != 0xFFFFFFFF):
            hashVal = self.HashMin + hashVal * (self.HashMax - self.HashMin) / 0xFFFFFFFF
        self.SeedValue += 1
        return hashVal

def main():
    """!
        Main function for file.
        Does Nothing.
    """
    print("Uuid Generate!")

if __name__ == "__main__":
    main()
