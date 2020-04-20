package org.serio.core.showcrawlerstorage;

import org.junit.Before;
import org.junit.Test;

import java.util.Collections;

import static org.junit.Assert.*;

public class CrawlerStepTest {
    private String attributeName;
    private String attributeValue;

    @Before
    public void setUp() throws Exception {
        attributeName = "url";
        attributeValue = "http://movies.com";
    }

    @Test
    public void shouldCreateStepWithoutAttributes() {
        // when
        CrawlerStep step = new CrawlerStep(CrawlerStep.Type.fetch);
        // then
        assertFalse(step.getAttribute(attributeName).isPresent());
    }

    @Test
    public void shouldCreateStepWithSpecifiedAttributes() {
        // when
        CrawlerStep step = new CrawlerStep(CrawlerStep.Type.fetch, Collections.singletonMap(attributeName, attributeValue));
        // then
        assertEquals(attributeValue, step.getAttribute(attributeName).orElse(null));
    }

    @Test
    public void shouldCreateStepOfTheSpecifiedType() {
        // when
        CrawlerStep step = new CrawlerStep(CrawlerStep.Type.transform);
        // then
        assertEquals(CrawlerStep.Type.transform, step.getType());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotCreateStepWithNoTypeSpecified() {
        // when
        new CrawlerStep(null);
    }

    @Test
    public void shouldCopyCrawlerStepWithTheNewAttributeSet() {
        // given
        CrawlerStep step = new CrawlerStep(CrawlerStep.Type.value);
        // when
        CrawlerStep stepWithAttribute = step.setAttribute(attributeName, attributeValue);
        // then
        assertEquals(attributeValue, stepWithAttribute.getAttribute(attributeName).orElse(null));
        assertFalse(step.getAttribute(attributeName).isPresent());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // when
        CrawlerStep step1 = new CrawlerStep(CrawlerStep.Type.fetch, Collections.singletonMap(attributeName, attributeValue));
        CrawlerStep step2 = new CrawlerStep(CrawlerStep.Type.fetch, Collections.singletonMap(attributeName, attributeValue));
        // then
        assertEquals(step1, step2);
        assertEquals(step1.hashCode(), step2.hashCode());
    }

    @Test
    public void shouldNotBeEqualDueToDifferentTypes() {
        // when
        CrawlerStep step1 = new CrawlerStep(CrawlerStep.Type.fetch);
        CrawlerStep step2 = new CrawlerStep(CrawlerStep.Type.value);
        // then
        assertNotEquals(step1, step2);
        assertNotEquals(step1.hashCode(), step2.hashCode());
    }

    @Test
    public void shouldNotBeEqualDueToDifferentAttributes() {
        // when
        CrawlerStep step1 = new CrawlerStep(CrawlerStep.Type.fetch);
        CrawlerStep step2 = new CrawlerStep(CrawlerStep.Type.fetch, Collections.singletonMap(attributeName, attributeValue));
        // then
        assertNotEquals(step1, step2);
        assertNotEquals(step1.hashCode(), step2.hashCode());
    }
}
