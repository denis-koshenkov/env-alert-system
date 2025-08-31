#include <stddef.h>

#include "alert_condition.h"
#include "eas_assert.h"
#include "config.h"
#include "variable_requirement_evaluator.h"

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#endif

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS
#define CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS 1
#endif

/**
 * @brief Variable requirements array size.
 *
 * Each AND in the condition is represented as a NULL pointer in the array. The array needs to fit at most
 * CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS variable requirements. The worst case scenario in terms of
 * storage is when we have CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS requirements, and each of them is
 * separated by an AND - NULL pointer.
 *
 * In that case, we would have (CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS - 1) NULL pointers
 * and CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS variable requirements. Hence, array size should be
 * (CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS + (CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS - 1))
 * to always be able to store CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS variable requirements.
 */
#define ALERT_CONDITION_VARIABLE_REQUIREMENTS_ARRAY_SIZE                                                               \
    (CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS + (CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS - 1))

struct AlertConditionStruct {
    VariableRequirement variable_requirements[ALERT_CONDITION_VARIABLE_REQUIREMENTS_ARRAY_SIZE];
    /** Number of variable requirements currently in the alert condition. This excludes the ANDs (NULL pointers), only
     * counts real variable requirements. */
    size_t num_requirements;
    /** Equal to the sum of num_requirements and the number of ANDs (NULL pointers in variable_requirements array).
     * Always equal to the current number of elements in the variable_requirements array. */
    size_t num_items_in_reqs_array;
    bool insert_and_before_next_requirement;
};

static struct AlertConditionStruct instances[CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

/**
 * @brief Add a variable requirement to the self->variable_requirements array.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 * @param variable_requirement Variable requirement to add to the list. It is allowed to be NULL - a NULL variable
 * requirement in the array represents a logial AND in the alert condition.
 */
static void add_requirement_to_list(AlertCondition self, VariableRequirement variable_requirement)
{
    EAS_ASSERT(self->num_items_in_reqs_array < ALERT_CONDITION_VARIABLE_REQUIREMENTS_ARRAY_SIZE);
    self->variable_requirements[self->num_items_in_reqs_array++] = variable_requirement;
}

/**
 * @brief Evaluate a ORed variable requirement.
 *
 * @param[in] self Alert condition instance returned by @ref alert_condition_create.
 * @param[in] start_idx Index for the self->variable_requirements array. Should point to the first variable requirement
 * of the ORed variable requirement that needs to be evaluated.
 * @param[out] end_idx This function writes to this parameter the index of the first variable requirement after the one
 * at @p start_idx that is not a part of this ORed requirement. There are two options:
 *   - This ORed variable requirement is not the last one in the alert condition. This means that there is a NULL
 * variable requirement in the array between this ORed variable requirement and the next one. In this case, @p end_idx
 * contains the index of that NULL variable requirement.
 *   - This ORed variable requirement is the last one in the alert condition. In that case, @p end_idx is set to
 * self->num_items_in_reqs_array, because self->num_items_in_reqs_array is the next index after the index of the last
 * variable requirement that is a part of this ORed requirement.
 *
 * @return true The ORed variable requirement evaluated to true.
 * @return false The ORed variable requirement evaluated to false.
 */
static bool evaluate_ored_requirement(AlertCondition self, size_t start_idx, size_t *end_idx)
{
    bool req_result = false;
    size_t req_idx = start_idx;

    /* The ORed requirement ends in two cases: we reached the end of the whole condition, or we reached a logical AND
     * (represented as NULL). */
    while ((req_idx < self->num_items_in_reqs_array) && (self->variable_requirements[req_idx] != NULL)) {
        /* An ORed requirement is a list of variable requirements that are ORed: (req OR req OR req ...). If one of them
         * already evaluated to true, we already know hat the whole ORed condition is true. It is then not necessary to
         * actually evaluate subsequent conditions. We still stay in the loop to reach the end of the ORed condition to
         * set end_idx to the correct value. */
        if (!req_result) {
            req_result = variable_requirement_evaluator_evaluate(self->variable_requirements[req_idx]);
        }
        req_idx++;
    }

    /* This assert fires if the loop body was never entered. This happens if start_idx points to a logical AND or is out
     * of bounds. */
    EAS_ASSERT(req_idx != start_idx);

    *end_idx = req_idx;
    return req_result;
}

AlertCondition alert_condition_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES);
    struct AlertConditionStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->num_requirements = 0;
    instance->num_items_in_reqs_array = 0;
    instance->insert_and_before_next_requirement = false;
    return instance;
}

void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement)
{
    bool is_num_allowed_requirements_exceeded =
        (self->num_requirements >= CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);
    EAS_ASSERT(!is_num_allowed_requirements_exceeded);

    if (self->insert_and_before_next_requirement) {
        /* Do not add NULL to the array as the first element - it is already clear that the first element is a start of
         * a new ORed requirement. */
        if (self->num_requirements > 0) {
            /* NULL represents logical AND in the list of requirements */
            add_requirement_to_list(self, NULL);
        }
        self->insert_and_before_next_requirement = false;
    }
    add_requirement_to_list(self, variable_requirement);
    self->num_requirements++;
}

void alert_condition_start_new_ored_requirement(AlertCondition self)
{
    self->insert_and_before_next_requirement = true;
}

bool alert_condition_evaluate(AlertCondition self)
{
    EAS_ASSERT(self->num_items_in_reqs_array > 0);

    bool ored_req_result;
    size_t req_idx = 0;

    while (req_idx < self->num_items_in_reqs_array) {
        ored_req_result = evaluate_ored_requirement(self, req_idx, &req_idx);
        if (!ored_req_result) {
            /* Condition is a list of ORed requirements that are ANDed: (ORed req) AND (ORed req) AND (ORed req) ...
            If one of ORed requirements evaluates to false, we already know that the whole condition is false. */
            return false;
        }

        /* evaluate_ored_requirement set req_idx to the index of the first condition that was not a part of the ORed
         * requirement. Now, req_idx is either pointing at an AND (represented as NULL), or it is equal to
         * self->num_items_in_reqs_array (if it was the last ORed requirement in the condition). We increment req_idx by
         * 1 to skip over the AND (NULL), so that in the next iteration we are pointing to the first requirement of the
         * next ORed requirement. If req_idx is already equal to self->num_items_in_reqs_array, then incrementing it by
         * 1 will not matter - the while loop condition will evaluate to false regardless and we will exit the loop. */
        req_idx++;
    }

    /* All ORed requirements evaluated to true -> condition evaluates to true */
    return true;
}

void alert_condition_reset(AlertCondition self)
{
    self->num_items_in_reqs_array = 0;
    self->num_requirements = 0;
    self->insert_and_before_next_requirement = false;
}
