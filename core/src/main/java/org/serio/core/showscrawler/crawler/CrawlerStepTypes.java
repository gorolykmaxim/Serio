package org.serio.core.showscrawler.crawler;

import java.util.*;

/**
 * Collection of all defined {@link CrawlerStep} types and their mandatory properties.
 */
public class CrawlerStepTypes {
    private final Map<String, List<String>> typeToMandatoryProperties;

    /**
     * Construct a collection of crawler step types.
     */
    public CrawlerStepTypes() {
        typeToMandatoryProperties = new HashMap<>();
    }

    /**
     * Add type to this collection.
     *
     * @param type                crawler step type to add
     * @param mandatoryProperties names of mandatory properties of the specified type
     */
    public void addType(String type, String... mandatoryProperties) {
        typeToMandatoryProperties.put(type, Arrays.asList(mandatoryProperties));
    }

    /**
     * Get all crawler step types, defined in this collection.
     *
     * @return all crawler step types
     */
    public Set<String> getAllTypes() {
        return typeToMandatoryProperties.keySet();
    }

    /**
     * Get collection of properties that are mandatory for a step with the specified type but which are
     * not present in the specified step.
     *
     * @param step step to look for properties in
     * @return collection of missing property names
     */
    public Collection<String> getMissingMandatoryPropertiesOf(CrawlerStep step) {
        List<String> mandatoryProperties = typeToMandatoryProperties.get(step.getType());
        if (mandatoryProperties == null) {
            throw new UnknownCrawlerStepTypeException(step.getType(), getAllTypes());
        }
        List<String> missingMandatoryProperties = new ArrayList<>();
        for (String mandatoryProperty : mandatoryProperties) {
            if (!step.getProperty(mandatoryProperty).isPresent()) {
                missingMandatoryProperties.add(mandatoryProperty);
            }
        }
        return missingMandatoryProperties;
    }
}
