require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#step" do
  
  ruby_version_is "" ... "1.9" do
    it "should require a block" do
      proc{DateTime.now.step(DateTime.now)}.should raise_error(LocalJumpError)
    end
  end

  ruby_version_is "1.9" do
    it "should return an enumerator without a block" do
      ds    = DateTime.civil(2008, 10, 11)
      de    = DateTime.civil(2008,  9, 29)
      e = de.step(ds)
      e.should be_kind_of(Enumerator)
      count = 0
      e.each do |d|
        d.should <= ds
        d.should >= de
        count += 1
      end.should == de
      count.should == 13
    end
  end

  it "should be able to step forward in time" do
    ds    = DateTime.civil(2008, 10, 11)
    de    = DateTime.civil(2008,  9, 29)
    count = 0
    de.step(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == de
    count.should == 13

    count = 0
    de.step(ds, 5) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == de
    count.should == 3

    count = 0
    de.step(ds, 0.25) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == de
    count.should == 49

    count = 0
    ds.step(de) do |d|; count += 1; end.should == ds
    count.should == 0

  end
  
  it "should be able to step backward in time" do
    ds    = DateTime.civil(2000, 4, 14)
    de    = DateTime.civil(2000, 3, 29)
    count = 0
    ds.step(de, -1) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == ds
    count.should == 17

    count = 0
    ds.step(de, -5) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == ds
    count.should == 4

    count = 0
    ds.step(de, -0.25) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == ds
    count.should == 65

    count = 0
    de.step(ds, -1) do |d|; count += 1; end.should == de
    count.should == 0
    
  end
  
  it "should yield once if the dates are the same, regardless of step" do
    ds = DateTime.civil(2008, 10, 11)
    count = 0
    ds.step(ds, 1) do |d|
      d.should == ds
      count += 1
    end.should == ds
    count.should == 1

    count = 0
    ds.step(ds, -1) do |d|
      d.should == ds
      count += 1
    end.should == ds
    count.should == 1
  end
  
  it "should not yield if the target date is greater than the receiver, and step is not positive" do
    ds = DateTime.civil(2008, 10, 11)
    count = 0
    ds.step(ds.next, -1) do |d|
      count += 1
    end.should == ds
    count.should == 0
  end
  
  it "should not yield if the target date is less than the receiver, and step is not negative" do
    ds = DateTime.civil(2008, 10, 11)
    count = 0
    ds.next.step(ds, 1) do |d|
      count += 1
    end.should == ds.next
    count.should == 0
  end
  
  it "should correctly handle offsets" do
    ds = DateTime.civil(2008, 10, 11)
    de = DateTime.civil(2008, 10, 11, 0, 0, 0, 0.5)

    count = 0
    ds.step(de, 0.25) do |d|
      count += 1
    end.should == ds
    count.should == 0

    count = 0
    ds.step(de, -0.25) do |d|
      count += 1
    end.should == ds
    count.should == 3

    count = 0
    de.step(ds, -0.25) do |d|
      count += 1
    end.should == de
    count.should == 0

    count = 0
    de.step(ds, 0.25) do |d|
      count += 1
    end.should == de
    count.should == 3

  end
  
  it "should raise an ArgumentError for a 0 step" do
    ds = DateTime.civil(2008, 10, 11)
    proc{ds.step(ds, 0){|d|}}.should raise_error(ArgumentError)
    proc{ds.step(ds+1, 0){|d|}}.should raise_error(ArgumentError)
    proc{ds.step(ds-1, 0){|d|}}.should raise_error(ArgumentError)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.step(c.jd + 2) do |d|
      d.should be_kind_of(c)
    end.should be_kind_of(c)
  end
end
