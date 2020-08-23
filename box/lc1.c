/*
 * Two failures:
 * 1. Preassume all values of nums array are positive number.
 * 2. Forget the life cycle of C stack values.
 *
 *
 */

/*
 * My solution.
 *
 */
int ret_array[2];

int* twoSum(int* nums, int numsSize, int target) {
    int i, j;

    
    for (i = 0; i < numsSize; i++) {
                for (j = i + 1; j < numsSize; j++)
                    if (target == nums[i] + nums[j])
                        goto out;
    }
    
out:    
    ret_array[0] = i;
    ret_array[1] = j;
    
    return ret_array;
}


